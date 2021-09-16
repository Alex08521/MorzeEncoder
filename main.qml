import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import MorzeEncoder 1.0
import QtQuick.Dialogs 1.3

Window {
    id: appWindow
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("MorzeEncoder")
    color: "lightgray"
    property bool lang: true

    FileDialog {
        id: fileDialog
        nameFilters: ["Text files (*.txt)", "All files (*)"]
        selectExisting: false
        onAccepted: {
            selectExisting === false ? encoder.saveToTXT(fileUrl) : encoder.loadFromTXT(fileUrl);
            selectExisting === true ? textQuestion.text = qsTr(encoder.questionStringRead()) : selectExisting = true;
        }
    }

    MessageDialog {
        id: messageDialog
        title: "Information"
        text: "The program determines the language of the text automatically,\nlanguage switching is necessary only in order to tell the program which language to translate\nMorse code into.\nYou can save and upload files only in the .txt format."
    }

    Encoder {
        id: encoder
    }

   ColumnLayout {
       id: coll
       spacing: 10
       anchors.fill: parent
       anchors.topMargin: parent.height/9
       anchors.rightMargin: 20
       anchors.leftMargin: parent.width - bLangChange.width - 10

       Rectangle {
           id: pageLanguage
           width: bLangChange.width
           height: bLangChange.height
           Layout.alignment: Qt.AlignRight
           Text {
               id: textLanguage
               anchors.centerIn: parent
               text: encoder.langStatus() ? "Русский" : "English"
               font.pixelSize: 16
           }
       }

       Button {
           id: bLangChange
           text: "Change language"
           font.pointSize: 10
           Layout.alignment: Qt.AlignRight
           onClicked: {
                encoder.changeLangStatus();
                textLanguage.text = encoder.langStatus() ? "Русский" : "English";
                bEncode.text = encoder.langStatus() ? "Декод." : "Encode";
                bSave.text = encoder.langStatus() ? "Сохр." : "Save";
                bLoad.text = encoder.langStatus() ? "Загр." : "Load";
                bInfo.text = encoder.langStatus() ? "Справка" : "Info";
                messageDialog.title = encoder.langStatus() ? "Справка" : "Information";
                messageDialog.text = encoder.langStatus() ? "Программа определяет язык текста автоматически,\nпереключение языка необходимо только для того,\nчто бы указать программе на какой язык певодить\nкод морзе.\nСохранять и загружать файлы можно только в формвте .txt." : "The program determines the language of the text automatically,\nlanguage switching is necessary only in order to tell the program which language to translate\nMorse code into.\nYou can save and upload files only in the .txt format.";
                bQuit.text = encoder.langStatus() ? "Выход" : "Quit";
           }
       }

       Button {
           id: bEncode
           text: "Encode"
           font.pointSize: 16
           Layout.alignment: Qt.AlignRight
           onClicked: {
                encoder.encode();
                textAnswer.text = qsTr(encoder.answerStringRead());
           }
       }

       Button {
           id: bSave
           text: "Save"
           font.pointSize: 16
           Layout.alignment: Qt.AlignRight
           onClicked: {
               fileDialog.selectExisting = false;
               fileDialog.open();
           }
       }

       Button {
           id: bLoad
           text: "Load"
           font.pointSize: 16
           Layout.alignment: Qt.AlignRight
           onClicked: {
               fileDialog.selectExisting = true;
               fileDialog.open();
           }
       }

       Button {
           id: bInfo
           text: "Info"
           font.pointSize: 16
           Layout.alignment: Qt.AlignRight
           onClicked: {
                messageDialog.open();
           }
       }

       Button {
           id: bQuit
           text: "Quit"
           font.pointSize: 16
           Layout.alignment: Qt.AlignRight
           onClicked: {
                appWindow.close()
           }
       }

       Item {
           Layout.fillHeight: true
       }
   }

   Rectangle {
       id: pageQuestion
       x: parent.width/10
       y: parent.height/10
       width: (parent.width/3)*2
       height: parent.height/3

       Flickable {
            id: flick1
            anchors.fill: parent
            width: parent.width
            height: parent.height
            clip: true
            focus: true
            ScrollBar.vertical: ScrollBar {id: scrollbar1; active: scrollbar1.active}

            function ensureVisible1(r)
            {
                 if (contentX >= r.x)
                      contentX = r.x;
                 else if (contentX+width <= r.x+r.width)
                      contentX = r.x+r.width-width;
                 if (contentY >= r.y)
                      contentY = r.y;
                 else if (contentY+height <= r.y+r.height)
                      contentY = r.y+r.height-height;
             }

             TextEdit {
                   id: textQuestion
                   text: qsTr(encoder.questionStringRead())
                   font.pixelSize: 14
                   width: flick1.width
                   focus: true
                   wrapMode: TextEdit.Wrap
                   onCursorRectangleChanged: flick1.ensureVisible1(cursorRectangle)
                   onTextChanged: {
                       encoder.changeQuestion(textQuestion.text);
                   }
              }
       }
   }

   Rectangle {
       id: pageAnswer
       x: pageQuestion.x
       y: pageQuestion.height + parent.height/6
       width: (parent.width/3)*2
       height: parent.height/3

       Flickable {
            id: flick2
            anchors.fill: parent
            width: parent.width
            height: parent.height
            clip: true
            focus: true
            Keys.onUpPressed: scrollbar2.decrease()
            Keys.onDownPressed: scrollbar2.increase()

            ScrollBar.vertical: ScrollBar {id: scrollbar2; active: scrollbar2.active}

           Text {
               id: textAnswer
               anchors.fill: parent
               text: qsTr(encoder.answerStringRead())
               font.pixelSize: 14
               width: flick2.width
               focus: true
               wrapMode: TextEdit.Wrap
           }
       }
   }

   Text {
       id: textQuestionLabel
       anchors.bottom: pageQuestion.top
       anchors.bottomMargin: 10
       anchors.left: parent.left
       anchors.leftMargin: 50
       text: qsTr("Your text:")
       font.pixelSize: 14
       width: flick2.width
       focus: true
       wrapMode: TextEdit.Wrap
   }

   Text {
       id: textAnswerLabel
       anchors.bottom: pageAnswer.top
       anchors.bottomMargin: 10
       anchors.left: parent.left
       anchors.leftMargin: 50
       text: qsTr("Decoded/Encoded text:")
       font.pixelSize: 14
       width: flick2.width
       focus: true
       wrapMode: TextEdit.Wrap
   }
}
