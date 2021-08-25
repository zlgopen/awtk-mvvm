ViewModel('devices', {
    data: {
      items: [],
      currentIndex: -1,
      unlocked: false
    },
    methods: {
      insertOne: function(index) {
        var item = {
          pack_type: Math.round(Math.random() * 1000) % 15,
          pack_params: Math.round(Math.random() * 1000) % 13,
          io1: Math.random() > 0.5,
          io2: Math.random() > 0.5,
          temp: Math.random() * 1000,
          a1: Math.random() * 1000,
          a2: Math.random() * 1000,
          tps: Math.round(Math.random() * 1000)
        }
        this.items.splice(index, 0, item);
      },
      canRemove: function(args) {
        var index = this.currentIndex;
        return index >= 0 && index < this.items.length;
      },
      remove: function(args) {
        var index = this.currentIndex;
        this.items.splice(index, 1);
        this.notifyItemsChanged(this.items);
      },
      canInsert: function(args) {
        var index = this.currentIndex;
        return index >= 0 && index <= this.items.length;
      },
      insert: function(args) {
        var index = this.currentIndex;
        this.insertOne(index);
        this.notifyItemsChanged(this.items);
      },
      canClear: function() {
        return this.items.length > 0;
      },
      clear: function() {
        this.items.splice(0, this.items.length);
        this.notifyItemsChanged(this.items);
      },
      canReset: function() {
        return this.items.length == 0;
      },
      reset: function() {
        for(var i = 0; i < 100; i++) {
          this.insertOne(this.items.length);
          this.insertOne(this.items.length);
          this.insertOne(this.items.length);
        }
        this.notifyItemsChanged(this.items);
      },
      canSetCurrent: function (args) {
        return true;
      },
      setCurrent: function (args) {
        console.log(args.index);
        this.currentIndex = args.index;
        this.notifyPropsChanged()
      },
      gpio_control_1: function (args) {
        console.log('gpio_control_1');
      },
      gpio_control_2: function (args) {
        console.log('gpio_control_2');
      }
    },
    onCreate: function(req) {
      console.log("devices create");
      for(var i = 0; i < 50; i++) {
        this.insertOne(this.items.length);
        this.insertOne(this.items.length);
        this.insertOne(this.items.length);
      }
    }
  });
  