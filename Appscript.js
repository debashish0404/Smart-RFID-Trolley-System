var ss = SpreadsheetApp.openById('183slM8a8-ybmTsszVjXVHN6hZICNNUu7qe5vCX3UVBQ');
var sheet = ss.getSheetByName('Sheet1');
var timezone = "Asia/Kolkata";

// Twilio
var accountSid = "**************************";
var authToken = "****************************";
var toNumber = "whatsapp:+**********************";
var fromNumber = "whatsapp:+141*********";

function doGet(e) {

  try {

    // RESET
    if (e.parameter.reset == "true") {
      sheet.clearContents();
      sheet.appendRow(["Name","Price","Qty","Total","Date","Time"]);
      return ContentService.createTextOutput("RESET DONE");
    }

    // BILL BUTTON
    if (e.parameter.bill == "true") {
      var msg = generateBill();
      sendWhatsApp(msg);
      return ContentService.createTextOutput("BILL SENT");
    }

    var name = e.parameter.name || "";
    var price = parseFloat(e.parameter.price) || 0;
    var mode = e.parameter.mode || "add";

    if (name == "") return ContentService.createTextOutput("No name");

    var now = new Date();
    var date = Utilities.formatDate(now, timezone, "yyyy-MM-dd");
    var time = Utilities.formatDate(now, timezone, "HH:mm:ss");

    var data = sheet.getDataRange().getValues();

    for (var i = 1; i < data.length; i++) {

      if (data[i][0] == name) {

        var qty = Number(data[i][2]) || 0;

        if (mode == "add") qty++;
        if (mode == "remove") qty--;

        if (qty <= 0) {
          sheet.deleteRow(i + 1);
          return ContentService.createTextOutput("Deleted");
        }

        sheet.getRange(i+1, 2).setValue(price);
        sheet.getRange(i+1, 3).setValue(qty);
        sheet.getRange(i+1, 4).setValue(price * qty);
        sheet.getRange(i+1, 5).setValue(date);
        sheet.getRange(i+1, 6).setValue(time);

        // 🔥 SAFE LIVE UPDATE (NOT FULL BILL)
        Utilities.sleep(300);
        sendWhatsApp("🛒 " + name + " Qty: " + qty);

        return ContentService.createTextOutput("Updated");
      }
    }

    // NEW ITEM
    if (mode == "add") {
      sheet.appendRow([name, price, 1, price, date, time]);

      Utilities.sleep(300);
      sendWhatsApp("🆕 " + name + " Added");
    }

    return ContentService.createTextOutput("Done");

  } catch (err) {
    return ContentService.createTextOutput("Error: " + err);
  }
}

// FULL BILL
function generateBill() {
  var data = sheet.getDataRange().getValues();

  var message = "Products:\n";
  var total = 0;

  for (var i = 1; i < data.length; i++) {
    var name = data[i][0];
    var price = data[i][1];
    var qty = data[i][2];

    var itemTotal = price * qty;
    total += itemTotal;

    message += "- " + name + ": " + price + " x " + qty + " = " + itemTotal + "\n";
  }

  message += "\nTotal Price: " + total;

  return message;
}

// WHATSAPP
function sendWhatsApp(msg) {

  var url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json";

  var payload = {
    "To": toNumber,
    "From": fromNumber,
    "Body": msg
  };

  var options = {
    "method": "post",
    "payload": payload,
    "headers": {
      "Authorization": "Basic " + Utilities.base64Encode(accountSid + ":" + authToken)
    }
  };

  UrlFetchApp.fetch(url, options);
}
