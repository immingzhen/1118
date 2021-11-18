import QtQuick 2.0
import QtLocation 5.8
import QtPositioning 5.8
import QtQuick.Controls 2.15




Rectangle{
    property double oldlat: 22.9086
    property double oldlng: 120.2058
    property var recentFiles: []
    property var tiptitle: ["date : ","ID : ","class : ", "length : ", "RF-ID : "]
    property var colortype: ["red","yellow","green","red","red", "blue"]
    property var animaltype: ["tuna","shark","marlin","tuna01","tuna02", "none"]
    property var animaltype0: ["tuna","shark","marlin","tuna01","tuna02", "none"]
    property var image_path: ["qrc:/tuna.png","qrc:/shark.png","qrc:/mar.png","qrc:/tuna001.png","qrc:/tuna002.png",""]
    Plugin{
        id:mapPlugin
        //name: "osm"
        name: "esri"


    }

    Map{
            id:mapView
            anchors.fill:parent
            plugin: mapPlugin
            center:QtPositioning.coordinate(oldlat,oldlng);
            zoomLevel:6
            //PluginParameter { name: "osm.mapping.custom.host"; value: "0cf39d2b3cf048539cdc2a4bca87cada" }
            activeMapType: supportedMapTypes[9]    //  esri:10(dark) 9(ocean), mapboxgl:1  2(eez)
            maximumZoomLevel :10.9


            //onCenterChanged: {


              //  h1.radius=5000.0*(6/(mapView.zoomLevel))*(6/(mapView.zoomLevel))*(6/(mapView.zoomLevel))*(6/(mapView.zoomLevel))*(6/(mapView.zoomLevel))*(6/(mapView.zoomLevel))
            //}
            MapCircle {
                id:h1
                visible: true
                center:QtPositioning.coordinate(oldlat,oldlng);
                radius: 5000.0*6/(mapView.zoomLevel)
                color: 'green'
                border.width: 3
                property string toolTipText: "ss \nmmmm"
                //ToolTip.text: toolTipText
                //ToolTip.visible: toolTipText ? ma.containsMouse : false
                ToolTip {
                    id: control
                    //property string toolTipText: "1mosdsdse \nmmmm"
                    text: h1.toolTipText

                    background: Rectangle {
                        border.color: "#21be2b"
                    }
                    visible: h1.toolTipText ? ma.containsMouse : false
                }
                //MouseArea {
                //       anchors.fill: parent
                //       onClicked: { console.log("5454 world") }
               // }
                MouseArea {
                    id: ma
                    anchors.fill: parent
                    hoverEnabled: true
                }





            }




            Component{
                   id: provider
                   //MapCircle{
                   //    visible:true
                   //    center:QtPositioning.coordinate(oldlat,oldlng+10);
                   //    radius: 500.0
                   //    color: 'green'
                   //    border.width: 3
                   //    property string toolTipText: " "
                   //    ToolTip.text: toolTipText

                       //ToolTip.visible: toolTipText ? msas.containsMouse : false
                       //MouseArea {
                      //     id: msas
                     //      anchors.fill: parent
                    //       hoverEnabled: true
                    //   }
                   //}
                   MapQuickItem{
                       id:hello
                       property string image_cc_path: "qrc:/mar.jpg"
                       visible: true
                       coordinate: QtPositioning.coordinate(24, 121);

                       sourceItem: Image {
                           id:hee
                           source: hello.image_cc_path
                           width: 80
                           height: 40
                       }
                       property string toolTipText: " "
                       ToolTip.text: toolTipText
                       ToolTip.visible: toolTipText ? msas.containsMouse : false
                       MouseArea {
                       id: msas
                       anchors.fill: parent
                       hoverEnabled: true
                       }
                       anchorPoint.x: hee.width/ 2
                       anchorPoint.y: hee.height/ 2
               }
               }


        }

        function setCenter(lat,lng,scale){
            mapView.center.latitude=lat
            mapView.center.longitude=lng

            if (scale != 0){
                mapView.zoomLevel =scale
            }
        }


        function remove(){

            //mapView.p.visible=false
            console.log(recentFiles.length)

            for (var i=0;i<recentFiles.length;i++){
             //mapView.removeMapItem(recentFiles[i])

             recentFiles[i].visible=false
             //recentFiles.pop()
            }
            recentFiles=[]


        }
        function testd(list_test){



            //mapView.p.visible=false
            console.log(list_test[0])
            var temp  = provider.createObject(mapView)
            temp.coordinate=QtPositioning.coordinate(list_test[5],list_test[6]);
            temp.ToolTip.text=tiptitle[0]+list_test[0];
            for (var i=0;i<4;i++){
              if (list_test[i+1]=="tuna"){
                  //list_test[i+1]="Yellowfin tuna"
                  temp.ToolTip.text=temp.ToolTip.text+"\n"+tiptitle[i+1]+"Yellowfin tuna";
              }
              else if (list_test[i+1]=="tuna01"){
                  //list_test[i+1]="Blackfin tuna"
                  temp.ToolTip.text=temp.ToolTip.text+"\n"+tiptitle[i+1]+"Blackfin tuna";
              }
              else if (list_test[i+1]=="tuna02"){
                  //list_test[i+1]="Bigeye tuna"
                  temp.ToolTip.text=temp.ToolTip.text+"\n"+tiptitle[i+1]+"Bigeye tuna";
              }
              else{
                  temp.ToolTip.text=temp.ToolTip.text+"\n"+tiptitle[i+1]+list_test[i+1];
              }


              //temp.ToolTip.text=temp.ToolTip.text+"\n"+tiptitle[i+1]+list_test[i+1];
            }
            temp.image_cc_path=search(list_test[2])
            //temp.color= search(list_test[2])
            //temp.border.color = search(list_test[2])
            mapView.addMapItem(temp)
            recentFiles.push(temp)
            //mapView.zoomLevel=mapView.zoomLevel+1
            //mapView.zoomLevel=mapView.zoomLevel-1

        }

        function search(txt)
        {
            console.log(txt)
            for (var i=0;i<=5;i++)
            {
                if (animaltype[i]==txt){
                    return image_path[i]
                }
            }

            return colortype[6]
        }

}
