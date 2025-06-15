FROM python:3.12-slim
WORKDIR /app
COPY . .
RUN pip install --upgrade pip && pip install . nuitka
RUN nuitka --standalone --onefile src/calculator/main.py -o app.bin
CMD ["./app.bin"]
