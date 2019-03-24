
function Books() {
  
}

Books.prototype = new Array();
Books.prototype.init = function(req) {
  this.push({
    name: "aaa",
    stock: 100
  });
  this.push({
    name: "aaa",
    stock: 100
  });

  return this;
}

function createBooks(req) {
  let books = new Books();

  return books.init(req);
}

