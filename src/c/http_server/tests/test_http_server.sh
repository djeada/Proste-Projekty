#!/bin/bash
set -e

# Start the server in the background
../build/main &
SERVER_PID=$!
sleep 1

# Test with curl
RESPONSE=$(curl -s http://localhost:8080)
EXPECTED="Hello, world!"

if [[ "$RESPONSE" == "$EXPECTED" ]]; then
  echo "Test passed: Received expected response."
  RESULT=0
else
  echo "Test failed: Unexpected response: $RESPONSE"
  RESULT=1
fi

# Cleanup
kill $SERVER_PID
exit $RESULT
