#!/bin/sh

# This file will export all variables defines in /etc/environment

set -a
source /etc/environment
set +a
