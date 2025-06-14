FROM python:3.12-slim
WORKDIR /app
COPY . .
RUN pip install --upgrade pip && pip install . nuitka
RUN nuitka --standalone --onefile src/example_package/example_module.py -o app.bin
CMD ["./app.bin"]
