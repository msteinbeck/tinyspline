### Build Docker Image
docker build -t tinyspline/build-deps:latest - < Dockerfile

### Release Docker Image
docker login -u tinyspline -p <PASSWORD>
docker tag <ID> tinyspline/build-deps:latest
docker push tinyspline/build-deps

### Change loader instruction
```shell
x86_64-apple-darwin17-install_name_tool -change \
	`x86_64-apple-darwin17-otool -L *tinysplinepython* | grep Python | cut -d' ' -f1 | sed -e 's/^[ \t]*//'` \
	Python \
	_tinysplinepython.so
```
