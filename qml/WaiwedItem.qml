import QtQuick 2.7
import QtQuick.Particles 2.0
import weathermodel 1.0
import QtMultimedia 5.9

Rectangle {
    id: root
    property real yPage: menuButton.state === "menu" ? appRoot.height / 1.6 : appRoot.height / 4.8
    color: weatherCommon.backgroundColor
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
        smooth: false
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
        ShaderEffect {
            id: videoBlendingShader
            visible: weatherCommon.showVideo
            property variant source: ShaderEffectSource {
                sourceItem: parent;
                //textureMirroring: ShaderEffectSource.MirrorHorizontally
                smooth: false
                hideSource: true
            }

            property color backgroundSourceColor: weatherCommon.backgroundColor
            anchors.fill: parent
            smooth: false
            blending: false
            onLogChanged: console.log(log)

            //change Alpha w/o changing color:
            //PC0 - original color
            //BC - background color
            //PA0 - original Alpha
            //PA1 - new Alpha
            //PC1 = (PC0*PA0 + BC*(1-PA0) - BC*(1-PA1)) / PA1
            fragmentShader: "
            precision mediump float;
            uniform sampler2D source;
            uniform float qt_Opacity;
            uniform lowp vec4 backgroundSourceColor;
            varying lowp vec2 qt_TexCoord0;

            float alphaCalc(float inpC, float keyC) {
                if ( keyC < 0.1 )
                    return inpC;
                else if ( inpC > keyC + 0.00001 )
                    return (inpC - keyC) / (1.0 - keyC);
                else if ( inpC < keyC - 0.00001 )
                    return (keyC - inpC) / keyC;
                else
                    return 0.0;
            }

            vec3 saturation(vec3 rgb, float adjustment) {
                const vec3 W = vec3(0.2125, 0.7154, 0.0721);
                vec3 intensity = vec3(dot(rgb, W));
                return mix(intensity, rgb, adjustment);
            }

            void main() {
                vec4 in_v = texture2D(source, qt_TexCoord0.xy);
                //vec4 color = vec4(0.21569, 0.54902, 0.0, 1.0);
                vec4 color = texture2D(source, vec2(0.01, 0.01));

                vec4 out_v = in_v;
                vec4 alpha;

                alpha.w = in_v.w;

                alpha.x = alphaCalc(in_v.x, color.x);
                alpha.y = alphaCalc(in_v.y, color.y);
                alpha.z = alphaCalc(in_v.z, color.z);

                if (alpha.x > alpha.y) {
                    if (alpha.x > alpha.z)
                        out_v.w = alpha.x;
                    else
                        out_v.w = alpha.z;
                } else if (alpha.y > alpha.z) {
                    out_v.w = alpha.y;
                } else {
                    out_v.w = alpha.z;
                }
                if (out_v.w > .00001) {
                    out_v.xyz = mix(backgroundSourceColor.xyz, (out_v.xyz - color.xyz) / out_v.www + color.xyz, out_v.w);
                    if (out_v.w > .2)
                        out_v.xyz = saturation(out_v.xyz, 0.2);

                    out_v.w *= alpha.w;
                } else {
                    out_v.xyz = backgroundSourceColor.xyz;
                }
                gl_FragColor = qt_Opacity*out_v;
            }"
        }
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
