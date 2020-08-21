### Build Docker Image
docker build -t tinyspline/build-deps:latest - < Dockerfile

### Release Docker Image
docker login -u tinyspline -p <PASSWORD>
docker tag <ID> tinyspline/build-deps:latest
docker push tinyspline/build-deps
