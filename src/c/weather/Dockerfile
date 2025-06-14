FROM gcc:latest
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y cmake curl && rm -rf /var/lib/apt/lists/*
RUN cmake -S . -B build && cmake --build build
CMD ["/app/build/main", "London"]
