ViewModel('calculator', {
  data: {
    expr: ''
  },
  methods: {
    addChar: function(args) {
      this.expr += args;
      this.notifyPropsChanged();
    },
    removeChar: function(args) {
      this.expr = this.expr.slice(0, -1);
      this.notifyPropsChanged();
    },
    eval: function(args) {
      this.expr = eval(this.expr).toString();
      this.notifyPropsChanged();
    }
  }
});
