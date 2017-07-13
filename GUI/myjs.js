var i = 0;


function getRequest() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("content1").innerHTML =
      this.responseText;
	  var serverResponse = this.responseText;
      alert(serverResponse); // Shows "15"
    }
  };
  var ans = document.getElementById("msg1").value; 
  var url = "http://localhost:8085/";
  
  xhttp.open("GET", url.concat(ans), true);
  xhttp.send(null);
  
}

function postRequest() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("content2").innerHTML = this.responseText;
    }
  };
  
  var ans = document.getElementById("msg2").value; 
  
  xhttp.open("POST", "http://localhost:8085", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhttp.send(ans);
}

function putRequest() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("content4").innerHTML = this.responseText;
    }
  };
  
  var ans = document.getElementById("msg4").value; 
  
  xhttp.open("PUT", "http://localhost:8085", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhttp.send(ans);
}

function deleteRequest() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("content3").innerHTML =
      this.responseText;
	  var serverResponse = this.responseText;
      alert(serverResponse); // Shows "15"
    }
  };
  var ans = document.getElementById("msg3").value; 
  var url = "http://localhost:8085/";
  
  xhttp.open("DELETE", url.concat(ans), true);
  xhttp.send(null);
}


function dragstart_handler(ev) {
	console.log("dragStart");

	// Add the id/class name of the drag source element to the drag data payload so
	// it is available when the drop event is fired
	ev.dataTransfer.setData("text", ev.target.className + "," + ev.target.id);
	
	// Tell the browser both copy and move are possible
	ev.effectAllowed = "copyMove";
}

function dragover_handler(ev) {
	console.log("dragOver");
	ev.preventDefault();
}

function drop_handler(ev) {
	console.log("Drop");
	var xVal= event.clientX;
	var yVal= event.clientY;
	ev.preventDefault();
	var data = event.dataTransfer.getData("text");
	//console.log(data);

	var className = data.split(",")[0];
	//console.log(className);
	
	// Get the id of drag source element (that was added to the drag data
	// payload by the dragstart event handler)
	var id = data.split(",")[1];
	//console.log(id);
	
	var n = className.search("move");
	
	// Only Move the element if the source and destination ids are both "move"
	if (n != -1 && ev.target.id == "canvas")
	{
		console.log("move");
		var node =  document.getElementById(id);
		node.style.left = xVal + 'px';
		node.style.top = yVal + 'px';
		ev.target.appendChild(node);
	}
	
	var n = className.search("copy");
	
	// Copy the element if the source and destination ids are both "copy"
	if (n != -1 && ev.target.id == "canvas") 
	{
		console.log("copy");
		var nodeCopy = document.getElementById(id).cloneNode(true);
		nodeCopy.id = "src_move" + '_' + parseInt(i) ;
		nodeCopy.className = "src_move";
		i++;
		ev.target.appendChild(nodeCopy);
	}
}

function dragend_handler(ev) {
	console.log("dragEnd");
	// Remove all of the drag data
	ev.dataTransfer.clearData();
}


function createHTML()
{
	console.log("Clicked Create HTML button");
	var stylesheet = document.styleSheets[0];
	console.log(stylesheet);
	var doc = document.implementation.createHTMLDocument("New Document");
	var p = document.getElementById("canvas").cloneNode(true);
	try {
		doc.body.appendChild(p);
	} catch(e) {
    console.log(e);
	}
	var parent = doc.getElementById("canvas");
	var child = doc.getElementById("title");
	parent.removeChild(child);

	var frame = document.getElementById("theFrame");
	var destDocument = frame.contentDocument;
	var srcNode = doc.documentElement;
	var newNode = destDocument.importNode(srcNode, true);
	destDocument.replaceChild(newNode, destDocument.documentElement);
}
