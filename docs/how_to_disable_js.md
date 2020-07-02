# 如何去掉 javascript 支持

通常情况，我们并不需要 javascript 支持，可以禁用 javascript。修改 SConstruct，注释掉下面这行代码：

```
# os.environ['WITH_JS'] = 'true'
```
