#!/bin/bash -ex

SERVICE_NAME=WirelessMouseDesktop.service
# Disable exit on failure incase the service doesn't exist
set +e
systemctl stop "${SERVICE_NAME}"
systemctl disable "${SERVICE_NAME}"
set -e

cd build
cmake --install .

systemctl enable "${SERVICE_NAME}"
systemctl start "${SERVICE_NAME}"
