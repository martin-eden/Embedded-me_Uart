#!/bin/bash

# Uninstall library
arduino-cli lib uninstall me_Uart

# Uninstall dependencies
arduino-cli lib uninstall \
  me_BaseTypes
