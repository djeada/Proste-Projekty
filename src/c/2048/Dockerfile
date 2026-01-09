FROM gcc:14
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y cmake clang-tidy clang-format && rm -rf /var/lib/apt/lists/*
RUN cmake -S . -B build && cmake --build build
CMD ["./build/main"]
