import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 512; height: 128

    ShaderEffect {
        width: 512; height: 128
        property color animatedColor;
        SequentialAnimation on animatedColor {
            loops: Animation.Infinite

            ColorAnimation {
                from: "white"
                to: "black"
                duration: 500
            }

            ColorAnimation {
                from: "red"
                to: "blue"
                duration: 500
            }

            ColorAnimation {
                from: "green"
                to: "yellow"
                duration: 500
            }

            ColorAnimation {
                from: "gray"
                to: "purple"
                duration: 500
            }
        }

        blending: false
        fragmentShader: "
            varying mediump vec2 qt_TexCoord0;
            uniform lowp float qt_Opacity;
            uniform lowp vec4 animatedColor;

            void main() {
                gl_FragColor = animatedColor * qt_Opacity;
                if (qt_TexCoord0.x < 0.25) {
                    gl_FragColor.g = 0.0;
                }
            }
        "
    }
}
