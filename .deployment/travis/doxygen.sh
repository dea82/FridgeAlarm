#!/bin/bash

# This script will generate Doxygen documentation and
# deploy it to GitHub Pages.
#
# See README.md for further information how Travis CI
# is setup to push to GitHub Pages.



set -e #Exit with nonzero exit code if anything fails

# Path to travis encrypted key in repository
KEY_PATH=".deployment/travis/key-deploy.enc"
# Path to Makefile
MAKE_PATH="src/"
# Path to generated Doxygen documentation
DOXYGEN_PATH="src/doc/doxygen/"

# Commits to this branch will be deployed
SOURCE_BRANCH="develop"
# Deployed to this branch
TARGET_BRANCH="gh-pages"
# The deployment working directory
CLONE_DIR="gh-pages"

if [ "$TRAVIS_PULL_REQUEST" != "false" -o "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
    echo "INFO: Deployment is only made on develop branch."
    exit 0;
fi


REPO=`git config remote.origin.url`
# Make sure we get an uri that we can ssh to.
# This will look for string "https://github.com/ and
# replace it with "git@github.com:"
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

# Generate documentation
make -C $MAKE_PATH doc

# Copy all generated documents to the repo location
cp -r $DOXYGEN_PATH* $CLONE_DIR

# Make sure to do a reset. Otherwise git things we're have
# deleted the files and replaced with unrelated new ones.
# By doing a reset we have the chance to see them as modified.
git -C $CLONE_DIR reset .

git -C $CLONE_DIR config user.name "Travis CI"
git -C $CLONE_DIR config user.email "andreas.laghamn@gmail.com"

# Find out if there are any differences
if [[ -z $(git -C $CLONE_DIR status --porcelain) ]]; then
    echo "No changes to documentation, do not deploy!"
    exit 0
fi

# Commit changes
git -C $CLONE_DIR add .
git -C $CLONE_DIR commit -m "Travis: Deploy to GitHub Pages ${SHA}"

# Travis encryption stuff, ENCTRYPTION_LABEL is found in .travis.yml
ENCRYPTED_KEY_VAR="encrypted_${ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}
openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in $KEY_PATH -out key-deploy -d
chmod 600 key-deploy
eval `ssh-agent -s`
ssh-add key-deploy
# Finally push the changes to repository
git -C $CLONE_DIR push $SSH_REPO $TARGET_BRANCH

echo "Successfully deployed Doxygen documentation to GitHub Pages!"
