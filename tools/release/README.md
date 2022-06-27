### NuGet

Upload package to: nuget.org

### PyPI

Run the `pypi.sh` script and upload the packages with:

```shell
python -m twine upload pypi/*
```

### Maven

Run the `maven.sh` script and upload the signed zip file to:
https://oss.sonatype.org

### Luarocks

1. Clone `git@github.com:tinyspline/lua.git` (branch `gh-pages`).
2. Copy the binary rocks into the root directory of the repository.
3. Run:

```shell
luarocks-admin make-manifest .
```

4. Commit and push.

### Go

1. Clone `git@github.com:tinyspline/go.git`.
2. Run:

```
unzip -d go -o tinyspline/tinyspline-go.zip
```

3. Commit, create tag, and push.

### RubyGems

Upload packages with:

```shell
gem push tinyspline/<file>.gem
```
