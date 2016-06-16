#!/bin/bash
set -e #Exit with nonzero exit code if anything fails

# Commits to this branch will be deployed
SOURCE_BRANCH="develop"
# Deployed to this branch
TARGET_BRANCH="gh-pages"
# The deployment working directory
CLONE_DIR="gh-pages"

if [ "$TRAVIS_PULL_REQUEST" != "false" -o "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
    echo "Don't  on other branches than develop."
    exit 0;
fi

REPO=`git config remote.origin.url`
# Don't understand this...
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`

# Get exsisting documentation
git clone $REPO $CLONE_DIR
# If no doc branch is available create one (orphan)
git -C $CLONE_DIR checkout $TARGET_BRANCH || git -C $CLONE_DIR checkout --orphan $TARGET_BRANCH
# This will reset a fresh orphan branch to include nothing!
git -C $CLONE_DIR reset --hard
# This will remove all old files in the repo. --ignore-unmatch
# is necessary for the first time when the branch is created.
git -C $CLONE_DIR rm -rf --ignore-unmatch .
pwd
make -C . doc
# Copy all generated documents to the repo location
cp -r doc/doxygen/* $CLONE_DIR

git -C $CLONE_DIR config user.name "Travis CI"
git -C $CLONE_DIR config user.email "andreas.laghamn@gmail.com"

if [[ -z $(git -C $CLONE_DIR status --porcelain) ]]; then
    echo "No changes to documentation, do not deploy!"
    exit 0
fi

git -C $CLONE_DIR add .
git -C $CLONE_DIR commit -m "Deploy to GitHub Pages ${SHA}"

ENCRYPTED_KEY_VAR="encrypted_${ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}
openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in doc/deploy_key.enc -out deploy_key -d
chmod 600 deploy_key
eval `ssh-agent -s`
ssh-add deploy_key

git -C $CLONE_DIR push $SSH_REPO $TARGET_BRANCH

echo "Successfully deployed Doxygen documentation!"
