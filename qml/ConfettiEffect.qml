// SPDX-FileCopyrightText: Nheko Contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Particles

Item {
    id: effectRoot

    readonly property int maxLifespan: confettiEmitter.lifeSpan
    required property bool shouldEffectsRun

    function pulseConfetti()
    {
        confettiEmitter.pulse(effectRoot.height * 1.5)
    }

    function removeParticles()
    {
        particleSystem.reset()
    }

    visible: effectRoot.shouldEffectsRun

    ParticleSystem {
        id: particleSystem

        Component.onCompleted: stop();
        paused: !effectRoot.shouldEffectsRun
        running: effectRoot.shouldEffectsRun
    }

    Emitter {
        id: confettiEmitter

        group: "confetti"
        width: effectRoot.width * 3/4
        enabled: false
        anchors.horizontalCenter: effectRoot.horizontalCenter
        y: effectRoot.height
        emitRate: Math.min(400 * Math.sqrt(effectRoot.width * effectRoot.height) / 870, 1000)
        lifeSpan: 15000
        system: particleSystem
        maximumEmitted: 500
        velocityFromMovement: 8
        size: 16
        sizeVariation: 4
        velocity: PointDirection {
            x: 0
            y: -Math.min(450 * effectRoot.height / 700, 1000)
            xVariation: Math.min(4 * effectRoot.width / 7, 450)
            yVariation: 250
        }
    }

    ImageParticle {
        system: particleSystem
        groups: ["confetti"]
        source: Qt.resolvedUrl("res/confettiparticle.svg")
        rotationVelocity: 0
        rotationVelocityVariation: 360
        colorVariation: 1
        color: "white"
        entryEffect: ImageParticle.None
        xVector: PointDirection {
            x: 1
            y: 0
            xVariation: 0.2
            yVariation: 0.2
        }
        yVector: PointDirection {
            x: 0
            y: 0.5
            xVariation: 0.2
            yVariation: 0.2
        }
    }

    Gravity {
        system: particleSystem
        groups: ["confetti"]
        anchors.fill: effectRoot
        magnitude: 350
        angle: 90
    }
}
