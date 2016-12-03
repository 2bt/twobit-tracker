ticks = 8
frames = 800
MACRO bass
 offset           = 7 0
 panning          = -0.1
 puslewidth       = 0.2 0.3 0.4 0.5
 volume           = 1 1 1 0.8 0.6 0.4 0.2
 wave             = 0
MACRO crd
 pulsewidth       = 0.01 | +0.005
 release          = 0.5221
 wave             = 0
MACRO crd0 < crd
 offset           = | 7 7 3 3 0 0
 panning          = 0.4
MACRO crd1 < crd
 offset           = | 8 8 3 3 0 0
 panning          = -0.3
MACRO crd2 < crd
 offset           = | 7 7 4 4 0 0
 panning          = -0.2
MACRO fx
 offset           = 0 | +12 +0 +-12.1999998
 panning          = 0.5 | +-0.01
 volume           = 1 | +-0.01
 wave             = | 4 4 3
MACRO kick
 offset           = 60 10 6 0 -4 -5
 puslewidth       = 0.5
 volume           = 1 1 1 1 | +-0.3
 wave             = 4 0
MACRO ld2
 attack           = 0.0227
 offset           = -5 0
 panning          = -0.1
 pulsewidth       = 0.5
 release          = 2.0885
 resolution       = 1
 vibratodepth     = 0 0.1 0.2 0.3
 vibratospeed     = 0.12
 volume           = 1
 wave             = 1
MACRO lead
 offset           = 0
 panning          = 0.1
 pulsewidth       = 0.3
 vibratodepth     = 0 0 0 0 0 0 0.1 0.2 0.3
 vibratospeed     = 0.1
 volume           = 0.7
 wave             = 3 0
MACRO pd
 attack           = 0.0227
 decay            = 1.0442
 panning          = -0.2 | +0.02 +0.3 +-0.3
 pulsewidth       = 0.01 | +0.02
 release          = 1.0442
 resolution       = 20
 wave             = 1
MACRO pd037 < pd
 offset           = | 0 0 3 3 7 7 12 12
MACRO pd047 < pd
 offset           = | 0 0 4 4 7 7 12 12
 panning          = 0.3 | +-0.02 +-0.3 +0.3
MACRO snare
 offset           = 40 12 4 0 70
 puslewidth       = 0.5
 volume           = 1 1 1 1 1 | +-0.08
 wave             = 4 0 0 0 4
TABLE
 drum01......... bass01......... pad01..........
 drum01......... bass01......... pad01.......... fx01........... ............... f01............ a01............
 drum01......... bass01......... pad01.......... fx01........... crd01..........
 drum01......... bass01......... pad01.......... lead01......... crd01..........
 drum02......... bass01......... ............... lead02.........
PATTERN a01
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 === ..... .....
 F-6 ld2.. .....
 === ..... .....
 D#6 ld2.. .....
 F-6 ld2.. .....
 === ..... .....
 G-6 ld2.. .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-6 ld2.. .....
 D#6 ld2.. .....
 === ..... .....
 C-6 ld2.. .....
 === ..... .....
 C-6 ld2.. .....
 A#6 ld2.. .....
 G#6 ld2.. .....
 === ..... .....
 G-6 ld2.. .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
PATTERN bass01
 C-2 bass. .....
 ... ..... .....
 C-3 bass. .....
 C-3 bass. .....
 ... ..... .....
 D#3 bass. .....
 A#2 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G-2 bass. .....
 C-2 bass. .....
 ... ..... .....
 C-3 bass. .....
 C-3 bass. .....
 ... ..... .....
 D#3 bass. .....
 A#2 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G-2 bass. .....
 C-2 bass. .....
 ... ..... .....
 C-3 bass. .....
 C-3 bass. .....
 ... ..... .....
 D#3 bass. .....
 A#2 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G-2 bass. .....
 C-2 bass. .....
 ... ..... .....
 C-3 bass. .....
 C-3 bass. .....
 ... ..... .....
 D#3 bass. .....
 A#2 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G#1 bass. .....
 G#2 bass. .....
 ... ..... .....
 G-2 bass. .....
PATTERN crd01
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd0. .....
 ... ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd1. .....
 ... ..... .....
 G#4 crd2. .....
 ... ..... .....
 A#4 crd2. .....
 ... ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd0. .....
 ... ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd0. .....
 ... ..... .....
 === ..... .....
 C-5 crd1. .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 C-5 crd1. .....
 ... ..... .....
 D-5 crd1. .....
 ... ..... .....
 ... ..... .....
 === ..... .....
PATTERN drum01
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-3 snare .....
 C-2 kick. .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 C-2 kick. .....
 C-2 kick. .....
PATTERN drum02
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-2 kick. .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 C-2 kick. .....
 C-2 kick. .....
 ... ..... .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 ... ..... .....
 C-2 kick. .....
 ... ..... .....
 C-3 snare .....
 C-2 kick. .....
 ... ..... .....
 C-3 snare .....
 ... ..... .....
 C-2 kick. .....
 C-2 kick. .....
PATTERN f01
 C-5 lead. .....
 === ..... .....
 D-5 lead. .....
 D#5 lead. .....
 === ..... .....
 F-5 lead. .....
 === ..... .....
 G-5 lead. .....
 ... ..... .....
 === ..... .....
 G-5 lead. .....
 === ..... .....
 G#5 lead. .....
 G-5 lead. .....
 === ..... .....
 F-5 lead. .....
 === ..... .....
 G-5 lead. .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
PATTERN fx01
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 F-4 fx... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
PATTERN lead01
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 F-4 fx... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-5 lead. .....
 === ..... .....
 F-5 lead. .....
 === ..... .....
 D#5 lead. .....
 ... ..... .....
 ... ..... .....
 D-5 lead. .....
 ... ..... .....
 ... ..... .....
 C-5 lead. .....
 === ..... .....
PATTERN lead02
 D-5 lead. .....
 D#5 lead. .....
 === ..... .....
 D#5 lead. .....
 === ..... .....
 D-5 lead. .....
 C-5 lead. .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 F-4 fx... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
PATTERN pad01
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 C-5 pd037 .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 G#4 pd047 .....
 === ..... .....
 G-4 pd037 .....
 === ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 C-5 pd037 .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 G#4 pd047 .....
 === ..... .....
 G-4 pd037 .....
 === ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 C-5 pd037 .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 G#4 pd047 .....
 === ..... .....
 G-4 pd037 .....
 === ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 C-5 pd037 .....
 ... ..... .....
 === ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 ... ..... .....
 C-5 pd037 .....
 === ..... .....
 G#4 pd047 .....
 === ..... .....
 G-4 pd037 .....
 === ..... .....