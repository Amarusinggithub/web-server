#!/usr/bin/env bash

ROOT_DIR="$(pwd)"

PULL=false
ATTACH=false

for arg in "$@"; do
  case $arg in
    --pull)
      PULL=true
      shift
      ;;
    --attach)
      ATTACH=true
      shift
      ;;
    *)
      echo "invalid flag"
      ;;
  esac
done

if [[ $PULL == true ]]; then
  echo "Building image"
  docker build -t web_server .
elif [[ $ATTACH == true ]]; then
  echo "Attaching to container"
  docker exec -it web_server bash
else
  echo "starting http server in c container"
  docker run --rm -it --name="web_server" -v "${ROOT_DIR}/":/web_server -w="/web_server" -p 6969:6969 web_server
fi