#!/bin/bash
set -e #Exit with nonzero exit code if anything fails

SOURCE_BRANCH="develop"
TARGET_BRANCH="gh-pages"


if [ "$TRAVIS_PULL_REQUEST" != "false" -o "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
    echo "Don't deploy on other branches than develop."
    exit 0;
fi

REPO=`git config remote.origin.url`
# Don't understand this...
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`

rm -rf gh-repo-temp
git clone $REPO gh-repo-temp
cd gh-repo-temp
git checkout $TARGET_BRANCH || git checkout --orphan $TARGET_BRANCH
git rm -rf .
# Helps us get rid of hidden files.
git clean -fxd
cd ..
make doc
cd gh-repo-temp
cp -r ../doc/doxygen/* .
git reset .

git config user.name "Travis CI"
git config user.email "andreas.laghamn@gmail.com"

if [[ -z $(git status --porcelain) ]]; then
    echo "No changes to documentation, do not deploy!"
    exit 0
fi
	
git add .
git commit -m "Deploy to GitHub Pages ${SHA}"

ENCRYPTED_KEY_VAR="encrypted_${ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}
openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in ../doc/deploy_key.enc -out deploy_key -d
chmod 600 deploy_key
eval `ssh-agent -s`
ssh-add deploy_key

git push $SSH_REPO $TARGET_BRANCH
cd ..
rm -rf gh-repo-temp
