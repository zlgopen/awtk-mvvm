var gBooks = [];

gBooks.canRemove = function(index) {
  console.log(index);

  return index < gBooks.length;;
}

gBooks.remove = function(index) {
  gBooks.splice(index, 1);

  return RET_ITEMS_CHANGED;
}

gBooks.canSale = function(index) {
  return gBooks[index].stock > 0;
}

gBooks.sale = function(index) {
  gBooks[index].stock = gBooks[index].stock - 1;

  return RET_OBJECT_CHANGED;
}

gBooks.canClear= function(index) {
  return gBooks.length > 0;
}

gBooks.clear = function(index) {
  gBooks.length = 0;

  return RET_ITEMS_CHANGED;
}

gBooks.add = function() {
  var b = {
    name:"book" + Math.round(Math.random() * 1000),
    stock: Math.round(Math.random() * 100),
    style: gBooks.length%2 ? "odd" : "even"
  }

  gBooks.push(b);

  return RET_ITEMS_CHANGED;
}

function createBooks(req) {
  for(var i = 0; i < 100; i++) {
    gBooks.add();
    gBooks.add();
    gBooks.add();
  }
  return gBooks;
}

