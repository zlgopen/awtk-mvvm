var MvvmFactory = (function() {
  var valueConverters = {};
  var valueValidators = {};
  var viewModelTemplates = {};

  var noop = function() {};
  var sharedPropertyDefinition = {enumerable : true, configurable : true, get : noop, set : noop};

  var warn = function(msg, vm) {
    if (vm !== undefined) {
      console.log('[mvvm warn]: [view model "' + vm.$name + '"] ' + msg);
    } else {
      console.log('[mvvm warn]: ' + msg);
    }
  };

  var hasOwnProperty = function(obj, key) {
    return Object.prototype.hasOwnProperty.call(obj, key);
  };

  var isPlainObject = function(obj) {
    return obj.toString() === '[object Object]';
  };

  /**
   * 以 $ 或者 _ 开头的则为保留名称
   */
  var isReservedName = function(str) {
    var c = (str + '').charCodeAt(0);
    return c === 0x24 || c === 0x5F;
  };

  var proxy = function(target, sourceKey, key) {
    sharedPropertyDefinition.get = function proxyGetter() {
      return this[sourceKey][key];
    };
    sharedPropertyDefinition.set = function proxySetter(val) {
      this[sourceKey][key] = val;
    };
    Object.defineProperty(target, key, sharedPropertyDefinition);
  };

  var getData = function(data, vm) {
    try {
      return data.call(vm, vm);
    } catch (e) {
      handleError(e, vm, 'data()');
      return {};
    }
  };

  var polyfillBind = function(fn, ctx) {
    function boundFn(a) {
      var l = arguments.length;
      return l ? l > 1 ? fn.apply(ctx, arguments) : fn.call(ctx, a) : fn.call(ctx);
    }

    boundFn._length = fn.length;
    return boundFn;
  };

  var nativeBind = function(fn, ctx) {
    return fn.bind(ctx);
  };

  var bind = Function.prototype.bind ? nativeBind : polyfillBind;

  var initData = function(vm) {
    var data = vm.data;
    var methods = vm.methods;
    var props = vm.props;

    data = typeof data === 'function' ? getData(data, vm) : data || {};
    if (!isPlainObject(data)) {
      data = {};
      warn('data functions should return an object.', vm);
    }

    for (var key in data) {
      if (methods && hasOwnProperty(methods, key)) {
        warn('data "' + key + '" is already defined as a method.', vm);
      } else if (props && hasOwnProperty(props, key)) {
        warn('data "' + key + '" is already defined as a prop.', vm);
      } else if (!isReservedName(key)) {
        proxy(vm, 'data', key);
      }
    }
  };

  var createGetterInvoker = function(fn) {
    return function computedGetter() {
      return fn.call(this, this);
    }
  };

  var defineComputed = function(target, key, userDef) {
    if (typeof userDef === 'function') {
      sharedPropertyDefinition.get = createGetterInvoker(userDef);
      sharedPropertyDefinition.set = noop;
    } else {
      sharedPropertyDefinition.get = userDef.get ? createGetterInvoker(userDef.get) : noop;
      sharedPropertyDefinition.set = userDef.set || noop;
    }
    if (sharedPropertyDefinition.set === noop) {
      sharedPropertyDefinition.set = function() {
        warn(('computed data "' + key + '" was assigned to, but it has no setter.'), this);
      };
    }
    Object.defineProperty(target, key, sharedPropertyDefinition);
  };

  var initComputed = function(vm) {
    var computed = vm.computed;

    for (var key in computed) {
      var userDef = computed[key];
      var getter = typeof userDef === 'function' ? userDef : userDef.get;

      if (getter == null) {
        warn('computed data "' + key + '" miss getter.', vm);
      }

      if (!(key in vm)) {
        defineComputed(vm, key, userDef);
      } else {
        if (key in vm.data) {
          warn('computed data "' + key + '" is already defined in data.', vm);
        } else if (vm.props && key in vm.props) {
          warn('computed data "' + key + '" is already defined as a prop.', vm);
        }
      }
    }
  };

  var initMethods = function(vm) {
    var methods = vm.methods;
    var props = vm.props;

    for (var key in methods) {
      if (typeof methods[key] !== 'function') {
        warn('method "' + key + '" has type "' + (typeof methods[key]) +
                 '" in the definition. ' +
                 'Did you reference the function correctly?',
             vm);
      }

      if (props && hasOwnProperty(props, key)) {
        warn('method "' + key + '" has already been defined as a prop.', vm);
      } else if ((key in vm) && isReservedName(key)) {
        warn('method "' + key + '" conflicts with an existing instance method, ' +
             'please avoid defining methods that start with _ or $.');
      }

      vm[key] = typeof methods[key] !== 'function' ? noop : bind(methods[key], vm);
    }
  };

  var deepClone = function(data) {
    var o, i, n;
    if (Array.isArray(data)) {
      o = [];
      for (i = 0, n = data.length; i < n; i++) {
        o.push(deepClone(data[i]));
      }
    } else if (typeof data === 'object') {
      o = {};
      for (i in data) {
        o[i] = deepClone(data[i]);
      }
    } else {
      o = data;
    }

    return o;
  };

  return {
    registerViewModel: function(name, template) {
      if (hasOwnProperty(viewModelTemplates, name)) {
        warn('view model template named "' + name + '" is replaced.');
      }

      viewModelTemplates[name] = template;
    },
    createViewModel: function(name) {
      if (!hasOwnProperty(viewModelTemplates, name)) {
        return undefined;
      }

      var obj = viewModelTemplates[name];
      if (!obj.$single) {
        obj = deepClone(obj);
      }

      obj.$name = name;
      initData(obj);
      initComputed(obj);
      initMethods(obj);

      return obj;
    },
    registerValueConverter: function(name, converter) {
      if (hasOwnProperty(valueConverters, name)) {
        warn('value converter named "' + name + '" is replaced.');
      }

      valueConverters[name] = converter;
    },
    getValueConverter: function(name) {
      if (!hasOwnProperty(valueConverters, name)) {
        return undefined;
      }
      return valueConverters[name];
    },
    registerValueValidator: function(name, validator) {
      console.log(name);
      if (hasOwnProperty(valueValidators, name)) {
        warn('value validator named "' + name + '" is replaced.');
      }

      valueValidators[name] = validator;
    },
    getValueValidator: function(name) {
      if (!hasOwnProperty(valueValidators, name)) {
        return undefined;
      }
      return valueValidators[name];
    }
  }
}());

var ViewModel = function(name, obj) {
  MvvmFactory.registerViewModel(name, obj);
};

var ValueConverter = function(name, obj) {
  MvvmFactory.registerValueConverter(name, obj);
};

var ValueValidator = function(name, obj) {
  MvvmFactory.registerValueValidator(name, obj);
};
