#pragma once

#include <string>
#include <unordered_map>

#include "esphome/core/component.h"

namespace esphome {
namespace KM271 {

enum Buderus2017Ids {
    BW1HK1 = 0x8000,     //: "Betriebswerte 1 HK1"
    BW2HK1 = 0x8001,     //: "Betriebswerte 2 HK1"
    VSTHK1 = 0x8002,     //: "Vorlaufsolltemperatur HK1"       (Grad)
    VITHK1 = 0x8003,     //: "Vorlaufisttemperatur HK1"        (Grad)
    RSTHK1 = 0x8004,     //: "Raumsolltemperatur HK1"          (Grad)
    RITHK1 = 0x8005,     //: "Raumisttemperatur HK1"           (Grad)
    EOZHK1 = 0x8006,     //: "Einschaltoptimierungszeit HK1"
    AOZHK1 = 0x8007,     //: "Ausschaltoptimierungszeit HK1"
    PLHK1 = 0x8008,      //: "Pumpenleistung HK1"              (Grad)
    MSHK1 = 0x8009,      //: "Mischerstellung HK1"             (Grad)
    NB01 = 0x800a,       //: "nicht belegt"
    NB02 = 0x800b,       //: "nicht belegt"
    KLHK1_P10 = 0x800c,  //: "Heizkennlinie HK1 bei + 10 Grad" (Grad)
    KLHK1_P00 = 0x800d,  //: "Heizkennlinie HK1 bei 0 Grad"    (Grad)
    KLHK1_N10 = 0x800e,  //: "Heizkennlinie HK1 bei - 10 Grad" (Grad)
    NB03 = 0x800f,       //: "nicht belegt"
    NB04 = 0x8010,       //: "nicht belegt"
    NB05 = 0x8011,       //: "nicht belegt"

    BW1HK2 = 0x8112,     //: "Betriebswerte 1 HK2"
    BW2HK2 = 0x8113,     //: "Betriebswerte 2 HK2"
    VSTHK2 = 0x8114,     //: "Vorlaufsolltemperatur HK2"       (Grad)
    VITHK2 = 0x8115,     //: "Vorlaufisttemperatur HK2"        (Grad)
    RSTHK2 = 0x8116,     //: "Raumsolltemperatur HK2"          (Grad)
    RITHK2 = 0x8117,     //: "Raumisttemperatur HK2"           (Grad)
    EOZHK2 = 0x8118,     //: "Einschaltoptimierungszeit HK2"
    AOZHK2 = 0x8119,     //: "Ausschaltoptimierungszeit HK2"
    PLHK2 = 0x811a,      //: "Pumpenleistung HK2"
    MSHK2 = 0x811b,      //: "Mischerstellung HK2"
    NB06 = 0x811c,       //: "nicht belegt"
    NB07 = 0x811d,       //: "nicht belegt"
    KLHK2_P10 = 0x811e,  //: "Heizkennlinie HK2 bei + 10 Grad" (Grad)
    KLHK2_P00 = 0x811f,  //: "Heizkennlinie HK2 bei 0 Grad"    (Grad)
    KLHK2_N10 = 0x8120,  //: "Heizkennlinie HK2 bei - 10 Grad" (Grad)
    NB08 = 0x8121,       //: "nicht belegt"
    NB09 = 0x8122,       //: "nicht belegt"
    NB10 = 0x8123,       //: "nicht belegt"

    BW1WW = 0x8424,  //: "Betriebswerte 1 WW"
    BW2WW = 0x8425,  //: "Betriebswerte 2 WW"
    WWST = 0x8426,   //: "Warmwassersolltemperatur"        (Grad)
    WWIT = 0x8427,   //: "Warmwasseristtemperatur",        (Grad)
    OZWW = 0x8428,   //: "Warmwasseroptimierungszeit"
    LPWW = 0x8429,   //: "Ladepumpe"                       ["aus", "Ladepumpe",
                     //: "Warmwasserpumpe", "beide"]

    KVST = 0x882a,       //: "Kesselvorlaufsolltemperatur"     (Grad)
    KVIT = 0x882b,       //: "Kesselvorlaufisttemperatur"      (Grad)
    BET = 0x882c,        //: "Brennereinschalttemperatur"      (Grad)
    BAT = 0x882d,        //: "Brennerausschalttemperatur"      (Grad)
    KINT1 = 0x882e,      //: "Kesselintegral 1"
    KINT2 = 0x882f,      //: "Kesselintegral 2"
    KFEHL = 0x8830,      //: "Kesselfehler"
    KBETR = 0x8831,      //: "Kesselbetrieb"
    BANST = 0x8832,      //: "Brenneransteuerung"              ["aus", "an"]
    ABTMP = 0x8833,      //: "Abgastemperatur"                 (Grad)
    MODBSTELL = 0x8834,  //: "modulare Brenner Stellwert"
    NB11 = 0x8835,       //: "nicht belegt"
    BLZ1S2 = 0x8836,     //: "Brennerlaufzeit 1 Stunden 2"
    BLZ1S1 = 0x8837,     //: "Brennerlaufzeit 1 Stunden 1"
    BLZ1S0 = 0x8838,     //: "Brennerlaufzeit 1 Stunden 0"
    BLZ2S2 = 0x8839,     //: "Brennerlaufzeit 2 Stunden 2"
    BLZ2S1 = 0x883a,     //: "Brennerlaufzeit 2 Stunden 1"
    BLZ2S0 = 0x883b,     //: "Brennerlaufzeit 2 Stunden 0"

    AT = 0x893c,       //: "Aussentemperatur"                (Grad)
    ATD = 0x893d,      //: "gedaempfte Aussentemperatur"     (Grad)
    VVK = 0x893e,      //: "Versionsnummer VK"
    VNK = 0x893f,      //: "Versionsnummer NK"
    MODKENN = 0x8940,  //: "Modulkennung"
    NB12 = 0x8941,     //: "nicht belegt"
};

struct BuderusParamDesc {
    const char* name;
    const char* unit;
};

struct BuderusParamsHasher
{
    std::size_t operator()(const Buderus2017Ids val) const noexcept
    {
        return std::hash<uint16_t>{}(static_cast<uint16_t>(val));
    }
};

static const std::unordered_map<Buderus2017Ids, BuderusParamDesc, BuderusParamsHasher>
    buderus2017Params{
        {BW1HK1, {"Betriebswerte 1 HK1", ""}},
        {BW2HK1, {"Betriebswerte 2 HK1", ""}},
        {VSTHK1, {"Vorlaufsolltemperatur HK1", "°C"}},
        {VITHK1, {"Vorlaufisttemperatur HK1", "°C"}},
        {RSTHK1, {"Raumsolltemperatur HK1", "°C"}},
        {RITHK1, {"Raumisttemperatur HK1", "°C"}},
        {EOZHK1, {"Einschaltoptimierungszeit HK1", ""}},
        {AOZHK1, {"Ausschaltoptimierungszeit HK1", ""}},
        {PLHK1, {"Pumpenleistung HK1", "°C"}},
        {MSHK1, {"Mischerstellung HK1", "°C"}},
        {KLHK1_P10, {"Heizkennlinie HK1 bei + 10 Grad", "°C"}},
        {KLHK1_P00, {"Heizkennlinie HK1 bei 0 Grad", "°C"}},
        {KLHK1_N10, {"Heizkennlinie HK1 bei - 10 Grad", "°C"}},

        {BW1HK2, {"Betriebswerte 1 HK2", ""}},
        {BW2HK2, {"Betriebswerte 2 HK2", ""}},
        {VSTHK2, {"Vorlaufsolltemperatur HK2", "°C"}},
        {VITHK2, {"Vorlaufisttemperatur HK2", "°C"}},
        {RSTHK2, {"Raumsolltemperatur HK2", "°C"}},
        {RITHK2, {"Raumisttemperatur HK2", "°C"}},
        {EOZHK2, {"Einschaltoptimierungszeit HK2", ""}},
        {AOZHK2, {"Ausschaltoptimierungszeit HK2", ""}},
        {PLHK2, {"Pumpenleistung HK2", ""}},
        {MSHK2, {"Mischerstellung HK2", ""}},
        {KLHK2_P10, {"Heizkennlinie HK2 bei + 10 Grad", "°C"}},
        {KLHK2_P00, {"Heizkennlinie HK2 bei 0 Grad", "°C"}},
        {KLHK2_N10, {"Heizkennlinie HK2 bei - 10 Grad", "°C"}},

        {BW1WW, {"Betriebswerte 1 WW", ""}},
        {BW2WW, {"Betriebswerte 2 WW", ""}},
        {WWST, {"Warmwassersolltemperatur", "°C"}},
        {WWIT, {"Warmwasseristtemperatur", "°C"}},
        {OZWW, {"Warmwasseroptimierungszeit", ""}},
        {LPWW,{"Ladepumpe",""}}, // ["aus", "Ladepumpe", "Warmwasserpumpe", "beide"]

        {KVST, {"Kesselvorlaufsolltemperatur", "°C"}},
        {KVIT, {"Kesselvorlaufisttemperatur", "°C"}},
        {BET, {"Brennereinschalttemperatur", "°C"}},
        {BAT, {"Brennerausschalttemperatur", "°C"}},
        //    {KINT1, {"Kesselintegral 1", ""}},
        //    {KINT2, {"Kesselintegral 2", ""}},
        {KFEHL, {"Kesselfehler", ""}},
        {KBETR, {"Kesselbetrieb", ""}},
        {BANST, {"Brenneransteuerung", ""}},  // ["aus", "an"]
        {ABTMP, {"Abgastemperatur", "°C"}},
        {MODBSTELL, {"modulare Brenner Stellwert", ""}},
        {BLZ1S2, {"Brennerlaufzeit 1 Stunden 2", "h"}},
        {BLZ1S1, {"Brennerlaufzeit 1 Stunden 1", "h"}},
        {BLZ1S0, {"Brennerlaufzeit 1 Stunden 0", "h"}},
        {BLZ2S2, {"Brennerlaufzeit 2 Stunden 2", "h"}},
        {BLZ2S1, {"Brennerlaufzeit 2 Stunden 1", "h"}},
        {BLZ2S0, {"Brennerlaufzeit 2 Stunden 0", "h"}},

        {AT, {"Aussentemperatur", "°C"}},
        {ATD, {"Gedaempfte Aussentemperatur", "°C"}},
        {VVK, {"Versionsnummer VK", ""}},
        {VNK, {"Versionsnummer NK", ""}},
        {MODKENN, {"Modulkennung", ""}},
    };

}  // namespace KM271
}  // namespace esphome