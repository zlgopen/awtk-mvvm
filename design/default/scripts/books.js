ViewModel('books', {
  data: {
    items: []
  },
  methods: {
    _add: function () {
      var item = {
        name: "book" + Math.round(Math.random() * 1000),
        stock: Math.round(Math.random() * 100)
      }
      this.items.push(item);
    },
    add: function () {
      this._add();
      this.notifyItemsChanged(this.items);
    },
    canRemove: function(args) {
      return args.index < this.items.length;
    },
    remove: function(args) {
      this.items.splice(args.index, 1);
      this.notifyItemsChanged(this.items);
    },
    canSale: function(args) {
      return this.items[args.index].stock > 0;
    },
    sale: function(args) {
      this.items[args.index].stock = this.items[args.index].stock - 1;
      this.notifyPropsChanged();
    },
    canClear: function() {
      return this.items.length > 0;
    },
    clear: function() {
      this.items.splice(0, this.items.length);
      this.notifyItemsChanged(this.items);
    },
    canClear: function () {
      return this.items.length > 0;
    },
    clear: function () {
      this.items.splice(0, this.items.length);
      this.notifyItemsChanged(this.items);
    }
  },
  onCreate: function (req) {
    for (var i = 0; i < 100; i++) {
      this._add();
      this._add();
      this._add();
    }
  }
});
