ViewModel('address', {
  _data : {
    'Beijing' : {'Beijing' : [ 'Dongcheng', 'Xicheng', 'Chaoyang', 'Fengtai', 'Shijingshan', 'Haidian' ]},
    'Shanghai' : {'Shanghai' : [ 'Xuhui', 'Changning', 'Jingan', 'Putuo', 'Hongkou', 'Yangpu' ]},
    'Guangdong' : {
      'Guangzhou' : [ 'Tianhe', 'Huangpu', 'Liwan', 'Yuexiu', 'Haizhu' ],
      'Shenzhen' : [ 'Luohu', 'Futian', 'Nanshan', 'Baoan', 'Longgang' ]
    }
  },
  _province : 'Beijing',
  _city : 'Beijing',
  data : {
    country : 'Dongcheng',
    detail: ''
  },
  computed : {
    province_list : {
      get : function() {
        console.log(this._data);
        return Object.keys(this._data).join(';');
      }
    },
    city_list : {
      get : function() {
        return Object.keys(this._data[this.province]).join(';');
      }
    },
    country_list : {
      get : function() {
        return this._data[this.province][this.city].join(';');
      }
    },
    address : {
      get : function() {
        return this.province + ' ' + this.city + ' ' + this.country + ' ' + this.detail;
      }
    },
    province : {
      get : function() {
        return this._province;
      },
      set : function(val) {
        this._province = val;
        this._city = this.city_list.split(';')[0];
      }
    },
    city : {
      get : function() {
        return this._city;
      },
      set : function(val) {
        this._city = val;
        this.country = this.country_list.split(';')[0];
      }
    }
  }
});
