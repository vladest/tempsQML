import QtQuick 2.7
import QtQuick.Particles 2.0
import weathermodel 1.0
import QtMultimedia 5.6

Rectangle {
    id: root
    property real yPage: menu.state === "menu" ? appRoot.height / 1.6 : appRoot.height / 4.8
    color: weatherCommon.backgroundColor
    signal settingsClicked
    property alias menuButton: menu
    state: "none"

    //onColorChanged: console.log("color", color)
    Timer {
        id: pauseTimer
        interval: 1
        repeat: false
        onTriggered: mediaPlayer.pause();
    }

    MediaPlayer {
        id: mediaPlayer
        loops: 1
        source: weatherCommon.showVideo ? weatherCommon.currentVideo: ""
        onErrorChanged: console.log("video error", error, errorString)

        onStatusChanged: {
            console.log("video status", status)
            if (status === MediaPlayer.Loaded) {
                mediaPlayer.play()
            }
        }
        onPositionChanged: {
            if (mediaPlayer.position > 1000 && mediaPlayer.duration - mediaPlayer.position < 1100) {
                pauseTimer.start()
            }
        }
        onSourceChanged: {
            console.log("video", source)
        }
    }

    VideoOutput {
        id: videoOutput
        visible: weatherCommon.showVideo
        //anchors.centerIn: parent
        y: appRoot.height / 5
        width: parent.width
        height: width / (840/640)
        source: mediaPlayer
        fillMode: VideoOutput.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: {
                mediaPlayer.seek(0)
                mediaPlayer.play()
            }
        }
    }
    ShaderEffect {
        id: videoBlendingShader
        visible: weatherCommon.showVideo
        property variant source: ShaderEffectSource {
            sourceItem: videoOutput;
            textureMirroring: ShaderEffectSource.MirrorHorizontally
            smooth: false
            hideSource: true
        }
        //enabled: false

        property color backgroundSourceColor: weatherCommon.backgroundColor
        anchors.fill: videoOutput
        blending: false
        onLogChanged: console.log(log)

        fragmentShader: "
            uniform sampler2D source;
            uniform lowp float qt_Opacity;
            uniform vec4 backgroundSourceColor;
            varying vec2 qt_TexCoord0;

            float th = 4.0; // threshold
            float a2 = .8;
            float spill = 1.0;

            float getAlpha(vec4 c){
                // First Vlahos assumption: Gf <= a2Bf
                return 1.0 - th*(c.g-a2*(max(c.r,c.b)));
            }

            vec4 despill(vec4 c){
                /// Second Vlahos assumption: max (Gf - Bf,0) <= max(Bf - Rf, 0)
                float sub = max(c.g - mix(c.b, c.r, 0.45), 0.0);
                c.g -= sub;
                c.a -= smoothstep(0.25, 0.5, sub*c.a);

                //restore luminance (kind of, I slightly reduced the green weight)
                float luma = dot(c.rgb, vec3(0.350, 0.587,0.164));
                c.r += sub*c.r*2.0*.350/luma;
                c.g += sub*c.g*2.0*.587/luma;
                c.b += sub*c.b*2.0*.164/luma;

                return c;
            }

            void main() {
                vec2 uv = vec2(0,1) + vec2(1,-1) * qt_TexCoord0.xy;
                vec4 fg = texture2D(source, uv);
                vec4 ofg = fg;

                fg.a = clamp(getAlpha(fg), 0.0, 1.0);
                fg = despill(fg);

                gl_FragColor.rgb = ofg.rgb;
                gl_FragColor.rgb = backgroundSourceColor.rgb*(1.0-fg.a) + fg.rgb*fg.a;
            }"
    }

    //flash effect
    SequentialAnimation {
        id: rainAnimation
        //running only on certain weather conditions
        running: weatherModel.currentWeather.weather_codition_icon_id === "11d" ||
                 weatherModel.currentWeather.weather_codition_icon_id === "11n"
        loops: Animation.Infinite

        //the idea is to quick change opacity...
        PropertyAnimation {
            target: root
            property: "opacity"
            from: 0
            to: 1
            duration: 500;
            easing.type: Easing.OutElastic
        }

        //..and then random pause between 1 and 7 seconds between flashes
        PauseAnimation {
            duration: Math.ceil(Math.random() * 6000) + 1000
        }
    }

    //setup snow or rain particles
    //using particles subsystem
    Connections {
        target: weatherModel
        onCurrentWeatherChanged: {
            if (systemTrayIcon.available) {
                systemTrayIcon.setNewIcon("icons/"+weatherModel.currentWeather.weather_codition_icon_id + ".svg")
                systemTrayIcon.toolTip = weatherModel.currentWeather.weather_codition_description
                systemTrayIcon.show()
                systemTrayIcon.showMessage(weatherModel.currentWeather.weather_codition_name,
                                           weatherModel.currentWeather.weather_codition_description
                                           + " " + weatherCommon.roundup(weatherCommon.convertToCurrentScale(weatherModel.currentWeather.temp))
                                           + "\u00B0" + weatherCommon.temperatureScale, 0, 3000)
            }
            if (weatherModel.currentWeather.weather_codition_description.indexOf('rain') !== -1) {
                if (weatherModel.currentWeather.weather_codition_description.indexOf('light') !== -1) {
                    rainEmitter.emitRate = 5
                } else if (weatherModel.currentWeather.weather_codition_description.indexOf('heavy') !== -1) {
                    rainEmitter.emitRate = 20
                } else {
                    rainEmitter.emitRate = 10
                }
                root.state = "rain"
            } else if (weatherModel.currentWeather.weather_codition_name === 'Snow') {
                snowEmitter.emitRate = 10
                root.state = "snow"
            }

        }
    }

    ParticleSystem {
        id: sysRain
        running: root.state === "rain" && weatherCommon.showAnimation
        onRunningChanged: {
            if (!running)
                reset()
        }
    }

    Emitter {
        id: rainEmitter
        system: sysRain
        emitRate: 10
        lifeSpan: 2000
        velocity: AngleDirection {angle: 90; magnitude: 500;}
        width: parent.width
        height: yPage
    }

    ParticleSystem {
        id: sysSnow
        running: root.state === "snow" && weatherCommon.showAnimation
        onRunningChanged: {
            if (!running)
                reset()
        }
    }

    Emitter {
        id: snowEmitter
        system: sysSnow
        emitRate: 20
        lifeSpan: 5000
        //snow flakes slower than rain drops
        velocity: AngleDirection {angle: 90; magnitude: 50;}
        width: parent.width
        height: yPage
    }

    ItemParticle {
        id: snowparticles
        anchors.fill: parent
        system: sysSnow
        delegate: SnowFlake {}
        fade: true
    }

    ItemParticle {
        id: rainparticles
        anchors.fill: parent
        system: sysRain
        delegate: RainDrop {}
        fade: true
    }

    Behavior on color { PropertyAnimation { duration: 500; } }

    Text {
        id: clock
        height: appRoot.height / 24
        z: 100
        anchors { right: parent.right; top: parent.top; rightMargin: appRoot.width / 48; topMargin: appRoot.height / 48; }
        font.pixelSize: appRoot.height / 40
        color: "white"
        Timer {
            interval: 5000
            repeat: true
            triggeredOnStart: true
            running: true
            onTriggered: {
                clock.text = Qt.formatDateTime(new Date(), "hh:mm")
            }
        }
    }
    MenuClose {
        id: menu
        height: appRoot.height/24
        width: height
        z: 1000
        onStateChanged: {
            settingsClicked()
        }

        anchors { left: parent.left; top: parent.top; leftMargin: appRoot.width/28; topMargin: appRoot.height/28; }
        MouseArea {
            anchors.fill: parent
            onClicked: menu.state = (menu.state == "menu" ? "back" : "menu")
        }
    }

    Image {
        id: img
        width: parent.width
        y: root.yPage - 40// + sourceSize.height
        source: "images/background.png"
        Behavior on y { PropertyAnimation { duration: 630 } }
    }

    Rectangle {
        color: "white"
        width: parent.width
        height: parent.height - img.y - img.sourceSize.height
        anchors.bottom: parent.bottom
    }
}
