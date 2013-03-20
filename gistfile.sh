#!/bin/sh

git filter-branch --env-filter '
  if ["$GIT_COMMITTER_NAME" = "harbolkn" ];
  then
    GIT_COMMITTER_NAME = "harbolkn";
    GIT_AUTHOR_NAME = "harbolkn";
    GIT_COMMITER_EMAIL = "harbolkn@gmail.com";
    GIT_AUTHOR_EMAIL = "harbolkn@gmail.com";
    git commit-tree "$@";
  else
    git commit-tree "$@";
  fi' HEAD
