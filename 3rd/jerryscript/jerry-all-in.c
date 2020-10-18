
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#ifdef WIN32
#pragma comment(lib, "ws2_32")
#endif/*WIN32*/

#include "./jerryscript/jerry-ext/handler/handler-assert.c"
#include "./jerryscript/jerry-ext/handler/handler-resource-name.c"
#include "./jerryscript/jerry-ext/handler/handler-register.c"
#include "./jerryscript/jerry-ext/handler/handler-gc.c"
#include "./jerryscript/jerry-ext/handler/handler-print.c"
#include "./jerryscript/jerry-ext/module/module.c"
#include "./jerryscript/jerry-ext/arg/arg-transform-functions.c"
#include "./jerryscript/jerry-ext/arg/arg-js-iterator-helper.c"
#include "./jerryscript/jerry-ext/arg/arg.c"
#include "./jerryscript/jerry-ext/debugger/debugger-tcp.c"
#include "./jerryscript/jerry-ext/debugger/debugger-rp.c"
#include "./jerryscript/jerry-ext/debugger/debugger-ws.c"
#include "./jerryscript/jerry-ext/debugger/debugger-sha1.c"
#include "./jerryscript/jerry-ext/debugger/debugger-serial.c"
#include "./jerryscript/jerry-ext/debugger/debugger-common.c"
#include "./jerryscript/jerry-ext/handle-scope/handle-scope.c"
#include "./jerryscript/jerry-ext/handle-scope/handle-scope-allocator.c"
#include "./jerryscript/jerry-port/default/default-fatal.c"
#include "./jerryscript/jerry-port/default/default-external-context.c"
#include "./jerryscript/jerry-port/default/default-module.c"
#include "./jerryscript/jerry-port/default/default-date.c"
#include "./jerryscript/jerry-port/default/default-io.c"
#include "./jerryscript/jerry-port/default/default-debugger.c"
#include "jerryscript/jerry-core/api/jerry-debugger-transport.c"
#include "jerryscript/jerry-core/api/jerry-debugger.c"
#include "jerryscript/jerry-core/api/jerry-snapshot.c"
#include "jerryscript/jerry-core/api/jerry.c"
#include "jerryscript/jerry-core/debugger/debugger.c"
#include "jerryscript/jerry-core/ecma/base/ecma-alloc.c"
#include "jerryscript/jerry-core/ecma/base/ecma-gc.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-collection.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-conversion.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-errol.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-external-pointers.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-number.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-string.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers-value.c"
#include "jerryscript/jerry-core/ecma/base/ecma-helpers.c"
#include "jerryscript/jerry-core/ecma/base/ecma-init-finalize.c"
#include "jerryscript/jerry-core/ecma/base/ecma-lcache.c"
#include "jerryscript/jerry-core/ecma/base/ecma-literal-storage.c"
#include "jerryscript/jerry-core/ecma/base/ecma-module.c"
#include "jerryscript/jerry-core/ecma/base/ecma-property-hashmap.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-prototype-unscopables.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-function-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-function.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-generator-function.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-generator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-generator.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-async-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-bigint-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-bigint.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-dataview-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-dataview.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-generator-function.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-generator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-generator.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-global.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-handlers.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-date.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-error.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-json.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-sort.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-intrinsic.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-json.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-math.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-proxy.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-reflect.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-set-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-set-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-set.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string-iterator-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-symbol-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-symbol.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-type-error-thrower.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-weakmap-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-weakmap.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-weakset-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-weakset.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/ecma-builtins.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-bigint64array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-bigint64array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-biguint64array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-biguint64array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-helpers.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray-prototype.c"
#include "jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-arguments-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-array-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-arraybuffer-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-async-generator-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-big-uint.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-bigint-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-bigint.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-boolean-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-comparison.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-container-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-conversion.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-dataview-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-eval.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-exceptions.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-function-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-get-put-value.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-iterator-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-jobqueue.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-lex-env.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-number-arithmetic.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-number-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-objects-general.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-objects.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-promise-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-proxy-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-reference.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-regexp-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-string-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-symbol-object.c"
#include "jerryscript/jerry-core/ecma/operations/ecma-typedarray-object.c"
#include "jerryscript/jerry-core/jcontext/jcontext.c"
#include "jerryscript/jerry-core/jmem/jmem-allocator.c"
#include "jerryscript/jerry-core/jmem/jmem-heap.c"
#include "jerryscript/jerry-core/jmem/jmem-poolman.c"
#include "jerryscript/jerry-core/jrt/jrt-fatals.c"
#include "jerryscript/jerry-core/lit/lit-char-helpers.c"
#include "jerryscript/jerry-core/lit/lit-magic-strings.c"
#include "jerryscript/jerry-core/lit/lit-strings.c"
#include "jerryscript/jerry-core/parser/js/byte-code.c"
#include "jerryscript/jerry-core/parser/js/common.c"
#include "jerryscript/jerry-core/parser/js/js-lexer.c"
#include "jerryscript/jerry-core/parser/js/js-parser-expr.c"
#include "jerryscript/jerry-core/parser/js/js-parser-mem.c"
#include "jerryscript/jerry-core/parser/js/js-parser-module.c"
#include "jerryscript/jerry-core/parser/js/js-parser-statm.c"
#include "jerryscript/jerry-core/parser/js/js-parser-tagged-template-literal.c"
#include "jerryscript/jerry-core/parser/js/js-parser-util.c"
#include "jerryscript/jerry-core/parser/js/js-parser.c"
#include "jerryscript/jerry-core/parser/js/js-scanner-ops.c"
#include "jerryscript/jerry-core/parser/js/js-scanner-util.c"
#include "jerryscript/jerry-core/parser/js/js-scanner.c"
#include "jerryscript/jerry-core/parser/regexp/re-bytecode.c"
#include "jerryscript/jerry-core/parser/regexp/re-compiler.c"
#include "jerryscript/jerry-core/parser/regexp/re-parser.c"
#include "jerryscript/jerry-core/vm/opcodes-ecma-arithmetics.c"
#include "jerryscript/jerry-core/vm/opcodes-ecma-bitwise.c"
#include "jerryscript/jerry-core/vm/opcodes-ecma-relational-equality.c"
#include "jerryscript/jerry-core/vm/opcodes.c"
#include "jerryscript/jerry-core/vm/vm-stack.c"
#include "jerryscript/jerry-core/vm/vm-utils.c"
#include "jerryscript/jerry-core/vm/vm.c"
