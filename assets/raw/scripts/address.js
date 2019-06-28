
function Address() {
  this.province = '广东';
  this.city = '广州';
  this.country = '天河区';
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
    return Object.keys(this.data);
  }
})

Object.defineProperty(Address.prototype, 'city_list', {
  get: function () {
    return Object.keys(this.data[this.province]);
  }
})

Object.defineProperty(Address.prototype, 'country_list', {
  get: function () {
    return this.data[this.province][this.city];
  }
})

Object.defineProperty(Address.prototype, 'address', {
  get: function () {
    return this.province + this.city + this.country;
  }
})

function createAddress(req) {
  var address = new Address();

  return address;
}


function test() {
  var addr = createAddress()
  console.log(addr.province_list);
  console.log(addr.city_list);
  console.log(addr.country_list);
}

test();

