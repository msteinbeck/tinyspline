### NuGet

Upload package at: nuget.org

### PyPI

Upload packages with:

```shell
python -m twine upload pypi/*
```

### Maven

Upload signed zip file at: https://oss.sonatype.org

### Luarocks

1. Check out `gh-pages`.
2. Copy binary rocks into `luarocks`.
3. Run:

```shell
luarocks-admin make-manifest .
```
