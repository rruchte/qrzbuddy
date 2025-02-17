import QtQuick
import QtLocation
import QtPositioning
import QtQuick.Layouts
import QtQuick.Controls

Map {
    id: mapview
    anchors.fill: parent
    plugin: mapPlugin

    Plugin {
        id: mapPlugin
        name: "osm"
        // Directly specify the tile server, this avoids the "API KEY REQUIRED" watermark
        PluginParameter { name: "osm.mapping.host"; value: "https://tile.openstreetmap.org/" }
        // Be courteous, tell the OSM who we are
        PluginParameter { name: "osm.useragent"; value: "qtmap" }
    }

    property double latitude: 39.50
    property double longitude: -98.35
    property double stationLatitude: 0
    property double stationLongitude: 0
    property int currentMarker: -1
    property bool autoFit: true
    property int timeoutCount: 0

    property var deletedMarkers: [];

    center: QtPositioning.coordinate(latitude, longitude) // Default to center of US
    zoomLevel: 4 // Zoom at a level where the entire US is shown in the viewport

    /*
        When we directly specify the tile server in osm.mapping.host, a custom map type will be added to the
        end of the supportedMapTypes list, so we need to specify the last item in the list
    */
    activeMapType: supportedMapTypes[supportedMapTypes.length - 1]

    signal showCallsignDetail(call: string);

    function setCenterPosition(lat, lng)
    {
        mapview.center = QtPositioning.coordinate(lat, lng);
    }

    function setZoom(zoom)
    {
        zoomLevel = zoom;
    }

    function coorinateHasBeenDeleted(lat, lng)
    {
        for (var i = 0; i < deletedMarkers.length; i++) {
            if (deletedMarkers[i].latitude === lat && deletedMarkers[i].longitude === lng) {
                return true;
            }
        }

        return false;
    }

    function addLocationMarker(lat, lng)
    {
        if(coorinateHasBeenDeleted(lat, lng))
        {
            return;
        }

        var item = locationMarker.createObject(mapview, {
            coordinate:QtPositioning.coordinate(lat, lng)
        });

        addMapItem(item);

        setCenterPosition(lat, lng);
    }

    function addStationMarker(lat, lng)
    {
        for (var i = 0; i < mapItems.length; i++) {
            if (mapItems[i].hasOwnProperty("type") && mapItems[i].type == "station")
            {
                removeMapItem(mapItems[i]);
                break;
            }
        }

        var item = stationMarker.createObject(mapview, {
            coordinate:QtPositioning.coordinate(lat, lng)
        });

        addMapItem(item);

        stationLatitude = lat;
        stationLongitude = lng;

        setCenterPosition(lat, lng);
    }

    function addNamedLocationMarker(lat, lng, name)
    {
        if(coorinateHasBeenDeleted(lat, lng))
        {
            return;
        }

        var item = locationMarker.createObject(mapview, {
            coordinate:QtPositioning.coordinate(lat, lng),
            labelString: name
        });

        var exists = false;

        for (var i = 0; i < mapItems.length; i++) {
            if (mapItems[i].hasOwnProperty("coordinate") && mapItems[i].coordinate.latitude === lat && mapItems[i].coordinate.longitude === lng) {
                removeMapItem(mapItems[i]);
                exists = true;
                break;
            }
        }

        addMapItem(item);

        if(!exists && autoFit)
        {
            setCenterPosition(lat, lng);
        }
    }

    function addNamedLocationMarkerWithSnr(lat, lng, name, snr, rpt, lastHeardTimestamp)
    {
        if(coorinateHasBeenDeleted(lat, lng))
        {
            return;
        }

        var item = locationMarker.createObject(mapview, {
            coordinate:QtPositioning.coordinate(lat, lng),
            labelString: name,
            snrValue: snr,
            rptValue: rpt,
            lastHeard: lastHeardTimestamp
        });

        var exists = false;

        for (var i = 0; i < mapItems.length; i++) {
            if (mapItems[i].hasOwnProperty("coordinate") && mapItems[i].coordinate.latitude === lat && mapItems[i].coordinate.longitude === lng) {
                removeMapItem(mapItems[i]);
                exists = true;
                break;
            }
        }

        addMapItem(item);

        if(!exists && autoFit)
        {
            setCenterPosition(lat, lng);
        }
    }

    // Add a line to the map
    function addPolyline(coord1, coord2)
    {
        // Instantiate our simple polyline component, pass our two coordinates in the path property
        var item = polyline.createObject(mapview, {
            path: [
                coord1,
                coord2
            ]
        });

        // Add the polyline instance to the map
        addMapItem(item);
    }

    function removeLocationMarker(lat, lng)
    {
        for (var i = 0; i < mapItems.length; i++) {
            if (mapItems[i].hasOwnProperty("coordinate") && mapItems[i].coordinate.latitude === lat && mapItems[i].coordinate.longitude === lng) {
                removeMapItem(mapItems[i]);
                break;
            }
        }
    }

    function removePolyline()
    {
        for (var i = 0; i < mapItems.length; i++)
        {
            if (mapItems[i].hasOwnProperty("path"))
            {
                removeMapItem(mapItems[i]);
                break;
            }
        }
    }
    
    function zoomToFitItems()
    {
        if(mapItems.length === 1)
        {
            setCenterPosition(mapItems[0].coordinate.latitude, mapItems[0].coordinate.longitude)
            zoomLevel = 10;
        }
        else if(autoFit)
        {
            fitViewportToVisibleMapItems();
        }
    }

    function showMarkerMenu()
    {
        markerPopupMenu.show();
    }

    function disableCenterAnimation()
    {
        centerAnimation.enabled = false;
    }

    function enableCenterAnimation()
    {
        centerAnimation.enabled = true;
    }

    function calculateDistance(lat1, lon1, lat2, lon2)
    {
        var R = 6371; // Radius of the earth in km
        var dLat = deg2rad(lat2-lat1);  // deg2rad below
        var dLon = deg2rad(lon2-lon1);
        var a =
            Math.sin(dLat/2) * Math.sin(dLat/2) +
            Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) *
            Math.sin(dLon/2) * Math.sin(dLon/2)
        ;
        var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
        var d = R * c; // Distance in km

        // Just convert to miles for now
        d *= 0.621371;

        // Convert to
        d = (d >= 10) ? Math.round(d):(d).toFixed(2);
        return d;
    }

    function calculateAzimuth(lat1, lon1, lat2, lon2) {
        // Convert degrees to radians
        lat1 = deg2rad(lat1);
        lon1 = deg2rad(lon1);

        lat2 = deg2rad(lat2);
        lon2 = deg2rad(lon2);

        // Apply the spherical trigonometry formula
        const deltaLon = lon2 - lon1;
        const y = Math.sin(deltaLon) * Math.cos(lat2);
        const x = Math.cos(lat1) * Math.sin(lat2) - Math.sin(lat1) * Math.cos(lat2) * Math.cos(deltaLon);
        let azim = Math.atan2(y, x) * (180 / Math.PI);

        // Normalize to 0-360 range
        if (azim < 0) {
            azim += 360;
        }

        return (azim).toFixed(2);
    }

    function deg2rad(deg) {
        return deg * (Math.PI/180)
    }

    Timer {
        id: activityTimeout
        interval: 60000;
        running: false;
        repeat: false;
        onTriggered: {
            mapview.autoFit = true;
        }
    }

    function kickAutofitWatchdog()
    {
        mapview.autoFit = false;
        activityTimeout.restart();
    }

    TapHandler {
        gesturePolicy: TapHandler.ReleaseWithinBounds
        onDoubleTapped: function(eventPoint) {

            if(zoomLevel < maximumZoomLevel) {
                mapview.zoomLevel = Math.round(zoomLevel + 1)
            }

            var postZoomPoint = mapview.toCoordinate(eventPoint.position);

            mapview.center = QtPositioning.coordinate(postZoomPoint.latitude, postZoomPoint.longitude);

            mapview.kickAutofitWatchdog();
        }
    }

    Behavior on zoomLevel {
        id: zoomAnimation
        NumberAnimation { duration: 500; easing.type: Easing.OutQuad }
    }

    Behavior on center {
        id: centerAnimation
        CoordinateAnimation { duration: 500; easing.type: Easing.OutQuad }
    }

    // Define our map marker component
    Component
    {
        id: locationMarker
        MapQuickItem
        {
            property string labelString: ""
            property string lastHeard: ""
            property double distanceValue: 0;
            property double azimuthValue: 0;
            property int snrValue: -99
            property int rptValue: -99
            property color positive: "#005b00"
            property color negative: "#9b0000"
            property int origZ: 0
            property string type: "remote"
            id: locationMarkerItem
            anchorPoint.x: image.width / 2
            anchorPoint.y: image.height
            coordinate: position
            sourceItem: Image {
                id: image
                width: 32
                height: 32
                source: "qrc:/images/map-marker.svg"
                sourceSize: Qt.size(80, 80)
                smooth: true
                antialiasing: true

                Text {
                    id: label
                    y: -16
                    width: image.width
                    color: "black"
                    font.bold: true
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    Component.onCompleted: {
                        text = (rptValue !== -99) ? labelString+"*":labelString;

                        var signal = Math.max(snrValue, rptValue);
                        if(signal === -99)
                        {
                            color =  "black";
                        }
                        else
                        {
                            color = (signal > 0) ? locationMarkerItem.positive:locationMarkerItem.negative;
                        }

                        locationMarkerItem.z = 23
                        locationMarkerItem.origZ = 42
                    }
                }

                Rectangle {
                    id: detailCard
                    y: 36
                    x: -42
                    width: 116
                    height: 47
                    color: "#AAFFFFFF"
                    border.width: 1
                    border.color: "#AA000000"
                    border.pixelAligned: true
                    radius: 5
                    visible: false

                    ColumnLayout {
                        spacing: 5
                        width: 102
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.topMargin: 5
                        anchors.rightMargin: 5
                        anchors.bottomMargin: 5
                        anchors.leftMargin: 5
                        Layout.alignment: Qt.AlignCenter

                        Text {
                            id: snr
                            width: parent.width
                            font.bold: true
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignCenter
                            Component.onCompleted: {
                                text = (snrValue === -99) ? "" : "SNR: " + snrValue;
                                color = (snrValue > 0) ? locationMarkerItem.positive : locationMarkerItem.negative;
                            }
                        }

                        Text {
                            id: rpt
                            width: parent.width
                            font.bold: true
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignCenter
                            Component.onCompleted: {
                                text = (rptValue === -99) ? "" : "RPT: " + rptValue;
                                color = (rptValue > 0) ? locationMarkerItem.positive : locationMarkerItem.negative;
                            }
                        }

                        Text {
                            id: distance
                            width: parent.width
                            font.bold: true
                            font.pixelSize: 12.
                            horizontalAlignment: Text.AlignCenter
                            Component.onCompleted: {
                                distanceValue = 0;

                                if(mapview.stationLatitude !== 0 &&  mapview.stationLongitude !== 0)
                                {
                                    distanceValue = calculateDistance(mapview.stationLatitude, mapview.stationLongitude, coordinate.latitude, coordinate.longitude);
                                }

                                text = (distanceValue > 0) ? "Distance: " + distanceValue + " mi":"";
                            }
                        }

                        Text {
                            id: azimuth
                            width: parent.width
                            font.bold: true
                            font.pixelSize: 12.
                            horizontalAlignment: Text.AlignCenter
                            Component.onCompleted: {
                                azimuthValue = 0;

                                if(mapview.stationLatitude !== 0 &&  mapview.stationLongitude !== 0)
                                {
                                    azimuthValue = calculateAzimuth(mapview.stationLatitude, mapview.stationLongitude, coordinate.latitude, coordinate.longitude);
                                }

                                text = (azimuthValue > 0) ? "Azimuth: " + azimuthValue + "°":"";
                            }
                        }

                        Text {
                            id: last
                            width: parent.width
                            font.bold: true
                            font.pixelSize: 12.
                            horizontalAlignment: Text.AlignCenter
                            Component.onCompleted: {
                                var dateParts = lastHeard.split(" ");
                                text = (dateParts.length > 1) ? "Last heard: " + dateParts[1]:"";
                            }
                        }
                    }
                }
            }

            HoverHandler
            {
                id: hoverHandler
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                onHoveredChanged: {
                    if(hovered)
                    {
                        var rowCount = 0;
                        snr.visible = false;
                        rpt.visible = false;
                        distance.visible = false;
                        azimuth.visible = false;
                        last.visible = false;

                        if(rptValue !== -99)
                        {
                            rpt.visible = true;
                            rowCount++;
                        }
                        if(snrValue !== -99)
                        {
                            snr.visible = true;
                            rowCount++;
                        }
                        if(distanceValue !== 0)
                        {
                            distance.visible = true;
                            rowCount++;
                        }
                        if(azimuthValue !== 0)
                        {
                            azimuth.visible = true;
                            rowCount++;

                            addPolyline(QtPositioning.coordinate(locationMarkerItem.coordinate.latitude, locationMarkerItem.coordinate.longitude), QtPositioning.coordinate(mapview.stationLatitude, mapview.stationLongitude));
                        }
                        if(lastHeard !== "")
                        {
                            last.visible = true;
                            rowCount++;
                        }

                        detailCard.height = (rowCount * 26);
                        detailCard.visible = (rowCount > 0);

                        locationMarkerItem.z = locationMarkerItem.origZ + 1000;
                    }
                    else
                    {
                        detailCard.visible = false;
                        locationMarkerItem.z = locationMarkerItem.origZ
                        removePolyline();
                    }
                }
            }

            TapHandler {
                id: tapHandler
                gesturePolicy: TapHandler.WithinBounds
                onTapped: {
                    mapview.currentMarker = -1
                    for (var i = 0; i< mapview.mapItems.length; i++)
                    {
                        if (locationMarkerItem == mapview.mapItems[i])
                        {
                            mapview.currentMarker = i

                            /*
                                Only show the menu if there is a label
                                This is a lame way of disabling the menu on the detail window map
                                TODO come up with a smarter way of doing this
                             */
                            if(locationMarkerItem.labelString.length > 0)
                            {
                                markerPopupMenu.show();
                            }

                            break
                        }
                    }
                }
            }
        }
    }

    // Define our station map marker component
    Component
    {
        id: stationMarker
        MapQuickItem
        {
            property string type: "station"
            id: stationMarkerItem
            anchorPoint.x: image.width / 2
            anchorPoint.y: image.height
            coordinate: position
            sourceItem: Image {
                id: image
                width: 32
                height: 32
                source: "qrc:/images/map-marker-blue.svg"
                sourceSize: Qt.size(80, 80)
                smooth: true
                antialiasing: true
            }
        }
    }

    // Define our polyline component
    Component
    {
        id: polyline
        MapPolyline {
            id: polylineInstance
            line.width: 2
            line.color: '#2b39c0'
            layer.enabled: true
            layer.samples: 8
        }
    }

    Menu {
        id: markerPopupMenu
        property int currentMarker
        property int markersCount

        MenuItem {
            text: "Show Detail"
            onTriggered: {
                console.log(mapview.mapItems[mapview.currentMarker].labelString)
                mapview.showCallsignDetail(mapview.mapItems[mapview.currentMarker].labelString)
            }
        }

        MenuItem {
            text: "Delete Marker"
            onTriggered: function()
                {
                    deletedMarkers.push(QtPositioning.coordinate(mapview.mapItems[mapview.currentMarker].coordinate.latitude, mapview.mapItems[mapview.currentMarker].coordinate.longitude));
                    mapview.removeMapItem(mapview.mapItems[mapview.currentMarker]);
                }
        }

        function show()
        {
            markerPopupMenu.markersCount = mapview.mapItems.length
            markerPopupMenu.popup()
        }
    }

    Rectangle {
        id: zoomButtonContainer
        width: 41
        height: 90
        z: parent.z + 100
        color: "#AAFFFFFF"
        border.width: 1
        border.color: "#AA000000"
        border.pixelAligned: true
        radius: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.topMargin: 0
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 0

        ColumnLayout
        {
            spacing: 10
            width: 31
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.rightMargin: 5
            anchors.bottomMargin: 10
            anchors.leftMargin: 5
            Layout.alignment: Qt.AlignCenter

            Rectangle
            {
                id: zoomInTrigger
                Layout.alignment: Qt.AlignCenter
                color: "transparent"
                width: 31
                height: 30

                Image {
                    id: zoomInIcon
                    source: "qrc:/images/zoom_in.png"
                    opacity: .6
                    anchors.centerIn: zoomInTrigger
                }

                HoverHandler {
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                    cursorShape: Qt.PointingHandCursor
                }

                TapHandler {
                    enabled: zoomLevel < maximumZoomLevel
                    gesturePolicy: TapHandler.ReleaseWithinBounds
                    onTapped: zoomLevel = Math.round(zoomLevel + 1)
                }
            }

            Rectangle
            {
                id: zoomOutTrigger
                Layout.alignment: Qt.AlignCenter
                color: "transparent"
                width: 30
                height: 30

                Image {
                    id: zoomOutIcon
                    source: "qrc:/images/zoom_out.png"
                    opacity: .6
                    anchors.centerIn: zoomOutTrigger
                }

                HoverHandler {
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                    cursorShape: Qt.PointingHandCursor
                }

                TapHandler {
                    enabled: zoomLevel > minimumZoomLevel
                    gesturePolicy: TapHandler.ReleaseWithinBounds
                    onTapped: zoomLevel = Math.round(zoomLevel - 1)
                }
            }
        }
    }

    // Log out some debug info when we finish loading
    Component.onCompleted: {
        console.log("Map Types: ", supportedMapTypes)
        console.log("Active Map Type: ", activeMapType)
    }

    // Set up all of our interaction handlers
    PinchHandler {
        id: pinch
        target: null
        onActiveChanged: if (active) {
            startCentroid = toCoordinate(pinch.centroid.position, false)
        }
        onScaleChanged: (delta) => {
            zoomLevel += Math.log2(delta)
            alignCoordinateToPoint(startCentroid, pinch.centroid.position)
        }
        onRotationChanged: (delta) => {
            bearing -= delta
            alignCoordinateToPoint(startCentroid, pinch.centroid.position)
        }
        grabPermissions: PointerHandler.TakeOverForbidden
    }

    WheelHandler {
        id: wheel
        // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
        // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
        // and we don't yet distinguish mice and trackpads on Wayland either
        acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
            ? PointerDevice.Mouse | PointerDevice.TouchPad
            : PointerDevice.Mouse
        rotationScale: 1/120
        property: "zoomLevel"
    }

    DragHandler {
        id: drag
        target: null
        onTranslationChanged: function (delta) {
            pan(-delta.x, -delta.y)
            mapview.kickAutofitWatchdog();
        }
    }

    Shortcut {
        enabled: zoomLevel < maximumZoomLevel
        sequence: StandardKey.ZoomIn
        onActivated: {
            zoomLevel = Math.round(zoomLevel + 1)
            mapview.kickAutofitWatchdog();
        }
    }

    Shortcut {
        enabled: zoomLevel > minimumZoomLevel
        sequence: StandardKey.ZoomOut
        onActivated: {
            zoomLevel = Math.round(zoomLevel - 1)
            mapview.kickAutofitWatchdog();
        }
    }

    Shortcut {
        enabled: true
        sequence: StandardKey.MoveToNextChar
        onActivated: function()
        {
            var dx = 0;
            var dy = 0;
            dx = -width / 4;

            var mapCenterPoint = Qt.point(width / 2.0 - dx, height / 2.0 - dy);
            center = toCoordinate(mapCenterPoint);
            mapview.kickAutofitWatchdog();
        }
    }

    Shortcut {
        enabled: true
        sequence: StandardKey.MoveToPreviousChar
        onActivated: function()
            {
                var dx = 0;
                var dy = 0;
                dx = width / 4;

                var mapCenterPoint = Qt.point(width / 2.0 - dx, height / 2.0 - dy);
                center = toCoordinate(mapCenterPoint);
                mapview.kickAutofitWatchdog();
            }
    }

    Shortcut {
        enabled: true
        sequence: StandardKey.MoveToNextLine
        onActivated: function()
            {
                var dx = 0;
                var dy = 0;
                dy = -height / 4;

                var mapCenterPoint = Qt.point(width / 2.0 - dx, height / 2.0 - dy);
                center = toCoordinate(mapCenterPoint);
                mapview.kickAutofitWatchdog();
            }
    }

    Shortcut {
        enabled: true
        sequence: StandardKey.MoveToPreviousLine
        onActivated: function()
            {
                var dx = 0;
                var dy = 0;
                dy = height / 4;

                var mapCenterPoint = Qt.point(width / 2.0 - dx, height / 2.0 - dy);
                center = toCoordinate(mapCenterPoint);
                mapview.kickAutofitWatchdog();
            }
    }
}