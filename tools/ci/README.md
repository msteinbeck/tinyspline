### Build Docker Image
```shell
docker build -t tinyspline/build-deps:latest -f Dockerfile .
```

### Release Docker Image
```shell
docker login -u tinyspline -p <PASSWORD>
docker tag <IMAGE_ID> tinyspline/build-deps:latest
docker push tinyspline/build-deps
```
