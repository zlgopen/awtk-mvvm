ViewModel("books", {
  data: {
    categories: [],
    selectedCategory: -1,
    lastInsertedCategory: -1,
    items: [],
  },
  methods: {
    _addCategory: function (index) {
      var item = {
        name: "Category" + Math.round(Math.random() * 1000),
        booksCount: Math.round(Math.random() * 100),
      };
      if (index < 0 || index >= this.categories.length) {
        this.categories.push(item);
        return this.categories.length - 1;
      } else {
        this.categories.splice(index, 0, item);
        return index
      }
    },
    _addBook: function () {
      var item = {
        name: "Book" + Math.round(Math.random() * 1000),
        stock: Math.round(Math.random() * 100),
      };
      this.items.push(item);
    },
    addBook: function () {
      this._addBook();
      this.notifyItemsChanged(this.items);
    },
    canRemoveCurrentBook: function (args) {
      return parseInt(args.index) < this.items.length;
    },
    removeCurrentBook: function (args) {
      this.items.splice(parseInt(args.index), 1);
      this.notifyItemsChanged(this.items);
    },
    canSaleBook: function (args) {
      return this.items[args.index].stock > 0;
    },
    saleBook: function (args) {
      this.items[args.index].stock = this.items[args.index].stock - 1;
      this.notifyPropsChanged();
    },
    canClearBooks: function () {
      return this.items.length > 0;
    },
    clearBooks: function () {
      this.items.splice(0, this.items.length);
      this.notifyItemsChanged(this.items);
    },
    addCategory: function () {
      this._addCategory(-1);
      this.notifyItemsChanged(this.categories);
    },
    insertCategory: function () {
      this.lastInsertedCategory = this._addCategory(this.selectedCategory);
      if (this.selectedCategory >= 0) {
        this.selectedCategory = this.selectedCategory + 1;
      }
      this.notifyItemsChanged(this.categories);
    },
    canSetSelectedCategory: function (args) {
      return true;
    },
    setSelectedCategory: function (args) {
      this.selectedCategory = parseInt(args.index);
      this.notifyPropsChanged();
    },
    canRemoveSelectedCategory: function (args) {
      return (
        this.selectedCategory > 0 &&
        this.selectedCategory < this.categories.length
      );
    },
    removeSelectedCategory: function (args) {
      this.categories.splice(this.selectedCategory, 1);
      this.selectedCategory = -1;
      this.notifyItemsChanged(this.categories);
    },
    canIncBooksCount: function (args) {
      return true;
    },
    incBooksCount: function (args) {
      this.categories[args.index].booksCount =
        this.categories[args.index].booksCount + 1;
      this.notifyPropsChanged();
    },
    canDecBooksCount: function (args) {
      return this.categories[args.index].booksCount > 0;
    },
    decBooksCount: function (args) {
      this.categories[args.index].booksCount =
        this.categories[args.index].booksCount - 1;
      this.notifyPropsChanged();
    },
    canClearCategories: function () {
      return this.categories.length > 0;
    },
    clearCategories: function () {
      this.categories.splice(0, this.categories.length);
      this.notifyItemsChanged(this.categories);
    },
  },
  onCreate: function (req) {
    for (var i = 0; i < 10; i++) {
      this._addCategory(-1);
      this._addCategory(-1);
      this._addCategory(-1);
    }
    for (var i = 0; i < 50; i++) {
      this._addBook();
      this._addBook();
      this._addBook();
    }
  },
});
