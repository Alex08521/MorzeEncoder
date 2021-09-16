import QtQuick 2.12

Rectangle{
    id:encodebutton
    color: "lightgray"
    width: 80
    height: 30

    Text{
        id: buttonLabel
        width: 64
        height: 24
        text: "Encode"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeight: 1.1
        minimumPixelSize: 36
        minimumPointSize: 36
        fontSizeMode: Text.FixedSize
        anchors.centerIn: parent;
    }

    MouseArea{
        id: buttonMouseArea
        anchors.fill: parent
        onClicked: console.log(buttonLabel.text = "clicked", encodebutton.color = "white")
    }
}
