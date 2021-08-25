ViewModel('shape', {
  data: {
    type: 1,
    x: 1,
    y: 2,
    w: 3,
    h:4,
    name: "name",
    opacity: 50,
    align: 1
  },
  computed: {
    overview : {
      get : function () {
        return this.name + " type=" + this.type + " align=" + 
          this.align + " opacity=" + this.opacity + 
          " (" + this.x + "," + this.y + "," + this.w + "," + this.h + ")";
      }
    }
  },
  methods: {
    canSave: function(args) {
      return !!this.name;
    },
    save: function(args) {
      console.log("save:" + this.type);
    },
    changeType: function(args) {
      this.type = parseInt(args);
      this.notifyPropsChanged();
      console.log("changeType:" + this.type);
      return RET_OK;
    }
  }
});
