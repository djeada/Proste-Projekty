FROM gcc:latest
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*
RUN cmake -S . -B build && cmake --build build
# Run another effect with: docker run --rm -it terminal_effects /app/build/plasma
CMD ["/app/build/donut"]
