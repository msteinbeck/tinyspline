### Build Docker Image
```shell
docker build -t tinyspline/build-deps:latest - < Dockerfile
```

### Release Docker Image
```shell
docker login -u tinyspline -p <PASSWORD>
docker tag <IMAGE_ID> tinyspline/build-deps:latest
docker push tinyspline/build-deps
```

### Change loader instruction
```shell
x86_64-apple-darwin17-install_name_tool -change \
	`x86_64-apple-darwin17-otool -L *tinysplinepython* | grep Python | cut -d' ' -f1 | sed -e 's/^[ \t]*//'` \
	Python \
	_tinysplinepython.so
```
