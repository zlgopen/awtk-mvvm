
function Shape() {
  this.type = 1;
  this.x = 1;
  this.y = 2;
  this.w = 3;
  this.h =4;
  this.name = "noname";
  this.opacity = 50;
  this.align = 1;
}

Shape.prototype.save = function(args) {
  console.log("save:" + this.type);
  return true;
}

Shape.prototype.canSave = function(args) {
  return !!this.name;
}

Shape.prototype.setType = function(args) {
  this.type = parseInt(args);
  console.log("setType:" + this.type);
  this.notifyPropsChanged();

  return true;
}

Object.defineProperty(Shape.prototype, "overview", {
  get: function () {
    return this.name + " type=" + this.type + " align=" + 
      this.align + " opacity=" + this.opacity + 
      " (" + this.x + "," + this.y + "," + this.w + "," + this.h + ")";
  }
})

var shape = new Shape();

