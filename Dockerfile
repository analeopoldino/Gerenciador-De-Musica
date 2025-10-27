#Dockerfile
FROM gcc:latest
WORKDIR /app
COPY . .
RUN make
CMD ["./music_manager"]
