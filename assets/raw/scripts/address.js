function Address() {
  this._province = '广东';
  this._city = '广州';
  this._country = '天河区';

  this.data = {
    '北京': {
      '北京': ['东城区', '西城区', '朝阳区','丰台区','石景山区','海淀区','']
    }, 
    '上海': {
      '上海': ['黄浦区', '徐汇区','长宁区','静安区','普陀区','虹口区','杨浦区']
    }, 
    '广东': {
      '广州':['天河区','黄埔区','荔湾区','越秀区','海珠区'],
      '深圳':['罗湖区','福田区','南山区','宝安区','龙岗区']
    }
  };
}

Object.defineProperty(Address.prototype, 'province_list', {
  get: function () {
    return Object.keys(this.data).join(';');
  }
})

Object.defineProperty(Address.prototype, 'city_list', {
  get: function () {
    return Object.keys(this.data[this._province]).join(';');
  }
})

Object.defineProperty(Address.prototype, 'country_list', {
  get: function () {
    return this.data[this._province][this._city].join(';');
  }
})

Object.defineProperty(Address.prototype, 'address', {
  get: function () {
    return this._province + this._city + this._country;
  }
})

Object.defineProperty(Address.prototype, 'province', {
  get: function () {
    return this._province;
  },
  set: function (val) {
    this._province = val;
    this.city = this.city_list.split(';')[0]; 
  }
})

Object.defineProperty(Address.prototype, 'city', {
  get: function () {
    return this._city;
  },
  set: function (val) {
    this._city = val;
    this.country = this.country_list.split(';')[0]; 
  }
})

Object.defineProperty(Address.prototype, 'country', {
  get: function () {
    return this._country;
  },
  set: function (val) {
    return this._country = val;
  }
})


function createAddress(req) {
  return new Address();
}

