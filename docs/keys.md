# Keys

The header `<cedar/defs.h>` defines numerous key codes. These key codes are a
24-bit code based on the 16-bit `kb_lkey_t` from `<keypadc.h>`. Key codes not
defined in this header are possible; all key code definitions use the following
format.

|  Bits  | Meaning                                                      |
|:------:|--------------------------------------------------------------|
| `0:7`  | A bitmask used to access this key's value within its group   |
| `8:15` | The group of this key                                        |
| `16`   | Set if the 2nd key was toggled on when the key was pressed   |
| `17`   | Set if the alpha key was toggled on when the key was pressed |

The constants `CEDAR_2NDFLAG` and `CEDAR_ALPHAFLAG` can be used to check the state
of the 2nd and alpha keys in a keycode.

The predefined keycodes are listed below:

| Name                | Key                                               |
|---------------------|---------------------------------------------------|
| `CEDAR_KB_GRAPH`    | <span class="key light">graph</span>              |
| `CEDAR_KB_TRACE`    | <span class="key light">trace</span>              |
| `CEDAR_KB_ZOOM`     | <span class="key light">zoom</span>               |
| `CEDAR_KB_WINDOW`   | <span class="key light">window</span>             |
| `CEDAR_KB_YEQU`     | <span class="key light">y=</span>                 |
| `CEDAR_KB_2ND`      | <span class="key _2nd">2nd</span>                 |
| `CEDAR_KB_MODE`     | <span class="key dark">mode</span>                |
| `CEDAR_KB_DEL`      | <span class="key dark">del</span>                 |
| `CEDAR_KB_STO`      | <span class="key dark">sto&rarr;</span>           |
| `CEDAR_KB_LN`       | <span class="key dark">ln</span>                  |
| `CEDAR_KB_LOG`      | <span class="key dark">log</span>                 |
| `CEDAR_KB_SQUARE`   | <span class="key dark">x<sup>2</sup></span>       |
| `CEDAR_KB_RECIP`    | <span class="key dark">x<sup>-1</sup></span>      |
| `CEDAR_KB_MATH`     | <span class="key dark">math</span>                |
| `CEDAR_KB_ALPHA`    | <span class="key alpha">alpha</span>              |
| `CEDAR_KB_0`        | <span class="key light">0</span>                  |
| `CEDAR_KB_1`        | <span class="key light">1</span>                  |
| `CEDAR_KB_2`        | <span class="key light">2</span>                  |
| `CEDAR_KB_3`        | <span class="key light">3</span>                  |
| `CEDAR_KB_4`        | <span class="key light">4</span>                  |
| `CEDAR_KB_5`        | <span class="key light">5</span>                  |
| `CEDAR_KB_6`        | <span class="key light">6</span>                  |
| `CEDAR_KB_7`        | <span class="key light">7</span>                  |
| `CEDAR_KB_8`        | <span class="key light">8</span>                  |
| `CEDAR_KB_9`        | <span class="key light">9</span>                  |
| `CEDAR_KB_COMMA`    | <span class="key dark">,</span>                   |
| `CEDAR_KB_SIN`      | <span class="key dark">sin</span>                 |
| `CEDAR_KB_COS`      | <span class="key dark">cos</span>                 |
| `CEDAR_KB_TAN`      | <span class="key dark">tan</span>                 |
| `CEDAR_KB_APPS`     | <span class="key dark">apps</span>                |
| `CEDAR_KB_GRAPHVAR` | <span class="key dark">x,&#7451;,&theta;,n</span> |
| `CEDAR_KB_DECPNT`   | <span class="key light">.</span>                  |
| `CEDAR_KB_DOT`      | <span class="key light">.</span>                  |
| `CEDAR_KB_LPAREN`   | <span class="key dark">(</span>                   |
| `CEDAR_KB_RPAREN`   | <span class="key dark">)</span>                   |
| `CEDAR_KB_PRGM`     | <span class="key dark">prgm</span>                |
| `CEDAR_KB_STAT`     | <span class="key dark">stat</span>                |
| `CEDAR_KB_CHS`      | <span class="key light">(-)</span>                |
| `CEDAR_KB_VARS`     | <span class="key dark">vars</span>                |
| `CEDAR_KB_ENTER`    | <span class="key light">enter</span>              |
| `CEDAR_KB_ADD`      | <span class="key light">+</span>                  |
| `CEDAR_KB_SUB`      | <span class="key light">&minus;</span>            |
| `CEDAR_KB_MUL`      | <span class="key light">&times;</span>            |
| `CEDAR_KB_DIV`      | <span class="key light">&divide;</span>           |
| `CEDAR_KB_POWER`    | <span class="key dark">^</span>                   |
| `CEDAR_KB_CLEAR`    | <span class="key dark">clear</span>               |
| `CEDAR_KB_DOWN`     | <span class="key light">&#x2BC6;</span>           |
| `CEDAR_KB_LEFT`     | <span class="key light">&#x2BC7;</span>           |
| `CEDAR_KB_RIGHT`    | <span class="key light">&#x2BC8;</span>           |
| `CEDAR_KB_UP`       | <span class="key light">&#x2BC5;</span>           |

| Name                | Key                                                                                                                            |
|---------------------|--------------------------------------------------------------------------------------------------------------------------------|
| `CEDAR_KB_STATPLOT` | <span class="_2nd">stat plot</span> / <span class="key _2nd">2nd</span> + <span class="key light">y=</span>                    |
| `CEDAR_KB_TBLSET`   | <span class="_2nd">tblset</span> / <span class="key _2nd">2nd</span> + <span class="key light">window</span>                   |
| `CEDAR_KB_FORMAT`   | <span class="_2nd">format</span> / <span class="key _2nd">2nd</span> + <span class="key light">zoom</span>                     |
| `CEDAR_KB_CALC`     | <span class="_2nd">calc</span> / <span class="key _2nd">2nd</span> + <span class="key light">trace</span>                      |
| `CEDAR_KB_TABLE`    | <span class="_2nd">table</span> / <span class="key _2nd">2nd</span> + <span class="key light">graph</span>                     |
| `CEDAR_KB_QUIT`     | <span class="_2nd">quit</span> / <span class="key _2nd">2nd</span> + <span class="key dark">mode</span>                        |
| `CEDAR_KB_INS`      | <span class="_2nd">ins</span> / <span class="key _2nd">2nd</span> + <span class="key dark">del</span>                          |
| `CEDAR_KB_LINK`     | <span class="_2nd">link</span> / <span class="key _2nd">2nd</span> + <span class="key dark">x,&#7451;,&theta;,n</span>         |
| `CEDAR_KB_LIST`     | <span class="_2nd">list</span> / <span class="key _2nd">2nd</span> + <span class="key dark">stat</span>                        |
| `CEDAR_KB_TEST`     | <span class="_2nd">test</span> / <span class="key _2nd">2nd</span> + <span class="key dark">math</span>                        |
| `CEDAR_KB_ANGLE`    | <span class="_2nd">angle</span> / <span class="key _2nd">2nd</span> + <span class="key dark">apps</span>                       |
| `CEDAR_KB_DRAW`     | <span class="_2nd">draw</span> / <span class="key _2nd">2nd</span> + <span class="key dark">prgm</span>                        |
| `CEDAR_KB_DISTR`    | <span class="_2nd">distr</span> / <span class="key _2nd">2nd</span> + <span class="key dark">vars</span>                       |
| `CEDAR_KB_MATRIX`   | <span class="_2nd">matrix</span> / <span class="key _2nd">2nd</span> + <span class="key dark">x<sup>-1</sup></span>            |
| `CEDAR_KB_ARCSIN`   | <span class="_2nd">sin<sup>-1</sup></span> / <span class="key _2nd">2nd</span> + <span class="key dark">sin</span>             |
| `CEDAR_KB_ARCCOS`   | <span class="_2nd">cos<sup>-1</sup></span> / <span class="key _2nd">2nd</span> + <span class="key dark">cos</span>             |
| `CEDAR_KB_ARCTAN`   | <span class="_2nd">tan<sup>-1</sup></span> / <span class="key _2nd">2nd</span> + <span class="key dark">tan</span>             |
| `CEDAR_KB_PI`       | <span class="_2nd">&pi;</span> / <span class="key _2nd">2nd</span> + <span class="key dark">^</span>                           |
| `CEDAR_KB_SQRT`     | <span class="_2nd">&#8730;</span> / <span class="key _2nd">2nd</span> + <span class="key dark">x<sup>2</sup></span>            |
| `CEDAR_KB_EE`       | <span class="_2nd">EE</span> / <span class="key _2nd">2nd</span> + <span class="key dark">,</span>                             |
| `CEDAR_KB_LCURLY`   | <span class="_2nd">{</span> / <span class="key _2nd">2nd</span> + <span class="key dark">(</span>                              |
| `CEDAR_KB_RCURLY`   | <span class="_2nd">}</span> / <span class="key _2nd">2nd</span> + <span class="key dark">)</span>                              |
| `CEDAR_KB_EULER`    | <span class="_2nd">e</span> / <span class="key _2nd">2nd</span> + <span class="key light">&divide;</span>                      |
| `CEDAR_KB_POW10`    | <span class="_2nd">10<sup>x</sup></span> / <span class="key _2nd">2nd</span> + <span class="key dark">log</span>               |
| `CEDAR_KB_u`        | <span class="_2nd">u</span> / <span class="key _2nd">2nd</span> + <span class="key light">7</span>                             |
| `CEDAR_KB_v`        | <span class="_2nd">v</span> / <span class="key _2nd">2nd</span> + <span class="key light">8</span>                             |
| `CEDAR_KB_w`        | <span class="_2nd">w</span> / <span class="key _2nd">2nd</span> + <span class="key light">9</span>                             |
| `CEDAR_KB_LSQUARE`  | <span class="_2nd">[</span> / <span class="key _2nd">2nd</span> + <span class="key light">&times;</span>                       |
| `CEDAR_KB_RSQUARE`  | <span class="_2nd">]</span> / <span class="key _2nd">2nd</span> + <span class="key light">&minus;</span>                       |
| `CEDAR_KB_POWE`     | <span class="_2nd">e<sup>x</sup></span> / <span class="key _2nd">2nd</span> + <span class="key dark">ln</span>                 |
| `CEDAR_KB_L1`       | <span class="_2nd">L1</span> / <span class="key _2nd">2nd</span> + <span class="key light">1</span>                            |
| `CEDAR_KB_L2`       | <span class="_2nd">L2</span> / <span class="key _2nd">2nd</span> + <span class="key light">2</span>                            |
| `CEDAR_KB_L3`       | <span class="_2nd">L3</span> / <span class="key _2nd">2nd</span> + <span class="key light">3</span>                            |
| `CEDAR_KB_L4`       | <span class="_2nd">L4</span> / <span class="key _2nd">2nd</span> + <span class="key light">4</span>                            |
| `CEDAR_KB_L5`       | <span class="_2nd">L5</span> / <span class="key _2nd">2nd</span> + <span class="key light">5</span>                            |
| `CEDAR_KB_L6`       | <span class="_2nd">L6</span> / <span class="key _2nd">2nd</span> + <span class="key light">6</span>                            |
| `CEDAR_KB_RCL`      | <span class="_2nd">rcl</span> / <span class="key _2nd">2nd</span> + <span class="key dark">sto&rarr;</span>                    |
| `CEDAR_KB_MEM`      | <span class="_2nd">mem</span> / <span class="key _2nd">2nd</span> + <span class="key light">+</span>                           |
| `CEDAR_KB_CATALOG`  | <span class="_2nd">catalog</span> / <span class="key _2nd">2nd</span> + <span class="key light">0</span>                       |
| `CEDAR_KB_IMAG`     | <span class="_2nd"><i>i</i></span> / <span class="key _2nd">2nd</span> + <span class="key light">.</span>                      |
| `CEDAR_KB_ANS`      | <span class="_2nd">ans</span> / <span class="key _2nd">2nd</span> + <span class="key light">(-)</span>                         |
| `CEDAR_KB_ENTRY`    | <span class="_2nd">entry</span> / <span class="key _2nd">2nd</span> + <span class="key light">enter</span>                     |
| `CEDAR_KB_BRIGHTEN` | <span class="_2nd">&#9788;</span> / <span class="key _2nd">2nd</span> + <span class="key light">&#x2BC5;</span>                |
| `CEDAR_KB_DARKEN`   | <span class="_2nd"><small>&#9788;</small></span> / <span class="key _2nd">2nd</span> + <span class="key light">&#x2BC5;</span> |

| Name                | Key                                                                                                                |
|---------------------|--------------------------------------------------------------------------------------------------------------------|
| `CEDAR_KB_F1`       | <span class="alpha">f1</span> / <span class="key alpha">alpha</span> + <span class="key light">y=</span>           |
| `CEDAR_KB_F2`       | <span class="alpha">f2</span> / <span class="key alpha">alpha</span> + <span class="key light">window</span>       |
| `CEDAR_KB_F3`       | <span class="alpha">f3</span> / <span class="key alpha">alpha</span> + <span class="key light">zoom</span>         |
| `CEDAR_KB_F4`       | <span class="alpha">f4</span> / <span class="key alpha">alpha</span> + <span class="key light">trace</span>        |
| `CEDAR_KB_F5`       | <span class="alpha">f5</span> / <span class="key alpha">alpha</span> + <span class="key light">graph</span>        |
| `CEDAR_KB_A`        | <span class="alpha">A</span> / <span class="key alpha">alpha</span> + <span class="key dark">math</span>           |
| `CEDAR_KB_B`        | <span class="alpha">B</span> / <span class="key alpha">alpha</span> + <span class="key dark">apps</span>           |
| `CEDAR_KB_C`        | <span class="alpha">C</span> / <span class="key alpha">alpha</span> + <span class="key dark">prgm</span>           |
| `CEDAR_KB_D`        | <span class="alpha">D</span> / <span class="key alpha">alpha</span> + <span class="key dark">x<sup>-1</sup></span> |
| `CEDAR_KB_E`        | <span class="alpha">E</span> / <span class="key alpha">alpha</span> + <span class="key dark">sin</span>            |
| `CEDAR_KB_F`        | <span class="alpha">F</span> / <span class="key alpha">alpha</span> + <span class="key dark">cos</span>            |
| `CEDAR_KB_G`        | <span class="alpha">G</span> / <span class="key alpha">alpha</span> + <span class="key dark">tan</span>            |
| `CEDAR_KB_H`        | <span class="alpha">H</span> / <span class="key alpha">alpha</span> + <span class="key dark">^</span>              |
| `CEDAR_KB_I`        | <span class="alpha">I</span> / <span class="key alpha">alpha</span> + <span class="key dark">x<sup>2</sup></span>  |
| `CEDAR_KB_J`        | <span class="alpha">J</span> / <span class="key alpha">alpha</span> + <span class="key dark">,</span>              |
| `CEDAR_KB_K`        | <span class="alpha">K</span> / <span class="key alpha">alpha</span> + <span class="key dark">(</span>              |
| `CEDAR_KB_L`        | <span class="alpha">L</span> / <span class="key alpha">alpha</span> + <span class="key dark">)</span>              |
| `CEDAR_KB_M`        | <span class="alpha">M</span> / <span class="key alpha">alpha</span> + <span class="key light">&divide;</span>      |
| `CEDAR_KB_N`        | <span class="alpha">N</span> / <span class="key alpha">alpha</span> + <span class="key dark">log</span>            |
| `CEDAR_KB_O`        | <span class="alpha">O</span> / <span class="key alpha">alpha</span> + <span class="key light">7</span>             |
| `CEDAR_KB_P`        | <span class="alpha">P</span> / <span class="key alpha">alpha</span> + <span class="key light">8</span>             |
| `CEDAR_KB_Q`        | <span class="alpha">Q</span> / <span class="key alpha">alpha</span> + <span class="key light">9</span>             |
| `CEDAR_KB_R`        | <span class="alpha">R</span> / <span class="key alpha">alpha</span> + <span class="key light">&times;</span>       |
| `CEDAR_KB_S`        | <span class="alpha">S</span> / <span class="key alpha">alpha</span> + <span class="key dark">ln</span>             |
| `CEDAR_KB_T`        | <span class="alpha">T</span> / <span class="key alpha">alpha</span> + <span class="key light">4</span>             |
| `CEDAR_KB_U`        | <span class="alpha">U</span> / <span class="key alpha">alpha</span> + <span class="key light">5</span>             |
| `CEDAR_KB_V`        | <span class="alpha">V</span> / <span class="key alpha">alpha</span> + <span class="key light">6</span>             |
| `CEDAR_KB_W`        | <span class="alpha">W</span> / <span class="key alpha">alpha</span> + <span class="key light">&minus;</span>       |
| `CEDAR_KB_X`        | <span class="alpha">X</span> / <span class="key alpha">alpha</span> + <span class="key dark">sto&rarr;</span>      |
| `CEDAR_KB_Y`        | <span class="alpha">Y</span> / <span class="key alpha">alpha</span> + <span class="key light">1</span>             |
| `CEDAR_KB_Z`        | <span class="alpha">Z</span> / <span class="key alpha">alpha</span> + <span class="key light">2</span>             |
| `CEDAR_KB_THETA`    | <span class="alpha">&theta;</span> / <span class="key alpha">alpha</span> + <span class="key light">3</span>       |
| `CEDAR_KB_QUOTE`    | <span class="alpha">"</span> / <span class="key alpha">alpha</span> + <span class="key light">+</span>             |
| `CEDAR_KB_SPACE`    | <span class="alpha">&#x2423;</span> / <span class="key alpha">alpha</span> + <span class="key light">0</span>       |
| `CEDAR_KB_COLON`    | <span class="alpha">:</span> / <span class="key alpha">alpha</span> + <span class="key light">.</span>             |
| `CEDAR_KB_QUESTION` | <span class="alpha">?</span> / <span class="key alpha">alpha</span> + <span class="key light">?</span>             |
| `CEDAR_KB_SOLVE`    | <span class="alpha">solve</span> / <span class="key alpha">alpha</span> + <span class="key light">enter</span>     |

<style>
span.key {
	padding: 2px;
	border: solid 1px black;
	border-radius: 5px;
	display: inline-block;
	min-width: 1.5em;
	text-align: center;
	font-weight: bold;
}

span.key._2nd {
	color: white;
	background-color: #51BCFB;
}

span.key.alpha {
	color: white;
	background-color: #5AFA5A;
}

span.key.light {
	color: black;
	background-color: white;
}

span.key.dark {
	color: white;
	background-color: black;
}

span._2nd {
	color: #51BCFB;
}

span.alpha {
    color: #5AFA5A;
}
</style>
