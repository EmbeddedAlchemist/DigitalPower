
// #include "usbpd_dpm_user.h"
#include "usbpd_core.h"
#include "usbpd_pwr_if.h"
#include "usbpd.hpp"
#include "clamp.hpp"
#include "in_range.hpp"
#include "onboard_resource.hpp"
#include <algorithm>
#include <cmath>


USBPD_SNKRDO_TypeDef USBPD::build_rdo_from_pdo(USBPD_PDO_TypeDef pdo, std::uint32_t pdo_idx, float v_target) {
    USBPD_SNKRDO_TypeDef rdo;
    rdo.d32                                 = 0;
    rdo.GenericRDO.USBCommunicationsCapable = USBPD_CORE_PDO_USBCOMM_CAPABLE;
    rdo.GenericRDO.UnchunkedExtendedMessage = USBPD_CORE_PDO_UNCHUNK_NOT_SUPPORTED;
    rdo.GenericRDO.NoUSBSuspend             = USBPD_CORE_PDO_USBSUSP_NOT_SUPPORTED;
    rdo.GenericRDO.ObjectPosition           = pdo_idx + 1;

    float v_max_supp = .0f, v_min_supp = .0f, c_max_supp = .0f, p_max_supp;

    switch (pdo.GenericPDO.PowerObject) {
    case USBPD_CORE_PDO_TYPE_FIXED:
        v_min_supp = v_max_supp = pdo.SRCFixedPDO.VoltageIn50mVunits * 0.05f;
        c_max_supp              = pdo.SRCFixedPDO.MaxCurrentIn10mAunits * 0.01f;
        break;

    case USBPD_CORE_PDO_TYPE_VARIABLE:
        v_max_supp = pdo.SRCVariablePDO.MaxVoltageIn50mVunits * 0.05f;
        v_min_supp = pdo.SRCVariablePDO.MinVoltageIn50mVunits * 0.05f;
        c_max_supp = pdo.SRCFixedPDO.MaxCurrentIn10mAunits * 0.01f;
        break;

    case USBPD_CORE_PDO_TYPE_BATTERY:
        v_max_supp = pdo.SRCBatteryPDO.MaxVoltageIn50mVunits * 0.05f;
        v_min_supp = pdo.SRCBatteryPDO.MinVoltageIn50mVunits * 0.05f;
        p_max_supp = pdo.SRCBatteryPDO.MaxAllowablePowerIn250mWunits * 0.25f;
        break;

    case USBPD_CORE_PDO_TYPE_APDO:
        v_max_supp = pdo.SRCSNKAPDO.MaxVoltageIn100mV * 0.1f;
        v_min_supp = pdo.SRCSNKAPDO.MinVoltageIn100mV * 0.1f;
        c_max_supp = pdo.SRCSNKAPDO.MaxCurrentIn50mAunits * 0.05f;
        break;
    }

    switch (pdo.GenericPDO.PowerObject) {
    case USBPD_CORE_PDO_TYPE_FIXED:
    case USBPD_CORE_PDO_TYPE_VARIABLE:
        rdo.FixedVariableRDO.OperatingCurrentIn10mAunits =
            rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits = c_max_supp / 0.01f;
        break;

    case USBPD_CORE_PDO_TYPE_BATTERY:
        rdo.BatteryRDO.MaxOperatingPowerIn250mWunits =
            rdo.BatteryRDO.OperatingPowerIn250mWunits = p_max_supp / 0.25f;
        break;

    case USBPD_CORE_PDO_TYPE_APDO:
        rdo.ProgRDO.OperatingCurrentIn50mAunits = c_max_supp / 0.05f;
        rdo.ProgRDO.OutputVoltageIn20mV         = clamp(v_min_supp, v_target, v_max_supp) / 0.02f;
        break;
    }
    return rdo;
}

void USBPD::log_src_pdo(std::uint32_t idx, USBPD_PDO_TypeDef pdo) {
    switch (pdo.GenericPDO.PowerObject) {
    case USBPD_CORE_PDO_TYPE_FIXED: {
        console.debug("%u:      FIXED %.2fV %.2fA ", idx, (float)pdo.SRCFixedPDO.VoltageIn50mVunits * 0.05f, (float)pdo.SRCFixedPDO.MaxCurrentIn10mAunits * 0.01f);
        break;
    }
    case USBPD_CORE_PDO_TYPE_VARIABLE:
        console.debug("%u:   VARIABLE %.2fV-%.2fV %.2fA ", idx, (float)pdo.SRCBatteryPDO.MinVoltageIn50mVunits * 0.05f, (float)pdo.SRCVariablePDO.MaxVoltageIn50mVunits * 0.05f, (float)pdo.SRCVariablePDO.MaxCurrentIn10mAunits * 0.01f);

        break;
    case USBPD_CORE_PDO_TYPE_BATTERY:
        console.debug("%u:    BATTERY %.2fV-%.2fV %.2fW ", idx, (float)pdo.SRCBatteryPDO.MinVoltageIn50mVunits * 0.05f, (float)pdo.SRCBatteryPDO.MaxVoltageIn50mVunits * 0.05f, (float)pdo.SRCBatteryPDO.MaxAllowablePowerIn250mWunits * 0.25f);

        break;
    case USBPD_CORE_PDO_TYPE_APDO:
        console.debug("%u: ARGUMENTED %.2fV-%.2fV %.2fA", idx, (float)pdo.SRCSNKAPDO.MinVoltageIn100mV * 0.1f, (float)pdo.SRCSNKAPDO.MaxVoltageIn100mV * 0.1f, (float)pdo.SRCSNKAPDO.MaxCurrentIn50mAunits * 0.05f);

        break;
    default:
        console.debug("%u:  UNHANDLED 0x%08x", idx, pdo.d32);
        break;
    }
}

bool USBPD::update_voltage_setting() {
    console.debug("Update voltage...");
    if (stack_started == false) {
        console.debug("Stack not started");
        return false;
    }
    std::uint32_t best_matched_idx = 0;
    float v_err                    = 0.f;
    if (find_best_matched_src_pdo(src_pdo, voltage_target, voltage_min, voltage_max, &best_matched_idx, &v_err) == false) {
        console.error("Cannot find matched pdo.");
        return false;
    }
    console.debug("Following pdo selected, target is %.2f, err is %.2f", voltage_target, v_err);
    log_src_pdo(best_matched_idx, src_pdo[best_matched_idx]);
    auto rdo    = build_rdo_from_pdo(src_pdo[best_matched_idx], best_matched_idx, voltage_target);
    auto result = USBPD_OK == USBPD_PE_Send_Request(port_num, rdo.d32, src_pdo[best_matched_idx].GenericPDO.PowerObject);
    console.debug("Request has been %s.", result ? "ACCEPTED" : "REJECTED");
    return result;
}

bool USBPD::find_best_matched_src_pdo(const std::vector<USBPD_PDO_TypeDef> &src_pdo_lst, float voltage_target, float voltage_min, float voltage_max, std::uint32_t *idx, float *v_err_out) {
    if (src_pdo_lst.size() == 0)
        return false;
    std::uint32_t best_matched_idx = -1;
    float v_max_supp = .0f, v_min_supp = .0f;
    float v_err = INFINITY;
    for (std::uint32_t i = 0; i < src_pdo_lst.size(); i++) {
        switch (src_pdo_lst[i].GenericPDO.PowerObject) {
        case USBPD_CORE_PDO_TYPE_FIXED:
            v_min_supp = v_max_supp = src_pdo_lst[i].SRCFixedPDO.VoltageIn50mVunits * 0.05f;
            break;

        case USBPD_CORE_PDO_TYPE_VARIABLE:
            v_max_supp = src_pdo_lst[i].SRCVariablePDO.MaxVoltageIn50mVunits * 0.05f;
            v_min_supp = src_pdo_lst[i].SRCVariablePDO.MinVoltageIn50mVunits * 0.05f;
            break;

        case USBPD_CORE_PDO_TYPE_BATTERY:
            v_max_supp = src_pdo_lst[i].SRCBatteryPDO.MaxVoltageIn50mVunits * 0.05f;
            v_min_supp = src_pdo_lst[i].SRCBatteryPDO.MinVoltageIn50mVunits * 0.05f;
            break;

        case USBPD_CORE_PDO_TYPE_APDO:
            v_max_supp = src_pdo_lst[i].SRCSNKAPDO.MaxVoltageIn100mV * 0.1f;
            v_min_supp = src_pdo_lst[i].SRCSNKAPDO.MinVoltageIn100mV * 0.1f;
            break;
        default:
            continue;
            break;
        }
        float v_err_curr = .0f;

        auto compare_err_and_update = [&](float curr_err) {
            v_err_curr = curr_err;
            if (v_err_curr <= v_err) {
                best_matched_idx = i;
                v_err            = v_err_curr;
            }
        };

        if (in_range(v_min_supp, voltage_target, v_max_supp)) {
            compare_err_and_update(0); // best matched;
        } else {
            if (in_range(voltage_min, v_max_supp, voltage_max))
                compare_err_and_update(std::abs(voltage_target - v_max_supp));
            if (in_range(voltage_min, v_min_supp, voltage_max))
                compare_err_and_update(std::abs(v_min_supp - voltage_target));
        }
    }
    if (best_matched_idx == -1)
        return false;
    *idx       = best_matched_idx;
    *v_err_out = v_err;
    return true;
}


void USBPD::impl_rcv_src_pdo(uint8_t *ptr, std::uint32_t size) {
    std::size_t count = size / sizeof(USBPD_PDO_TypeDef);
    console.debug("Received %u pdo(s) from source:", count);
    src_pdo.clear();
    for (std::size_t i = 0; i < count; i++) {
        src_pdo.push_back(reinterpret_cast<USBPD_PDO_TypeDef *>(ptr)[i]);
    }
    for (std::size_t i = 0; i < src_pdo.size(); i++) {
        log_src_pdo(i, src_pdo[i]);
    }
}


void USBPD::impl_cable_attached() {
    console.debug("Cable attached.");
    cable_attached = true;
}

void USBPD::impl_cable_detached() {
    console.debug("Cable detached.");
    cable_attached = false;
}

void USBPD::impl_snk_eval_capabilities(uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType) {
    console.debug("Eval capabilities...");
    std::uint32_t best_matched_idx = 0;
    float v_err                    = .0f;
    if (find_best_matched_src_pdo(src_pdo, voltage_target, voltage_min, voltage_max, &best_matched_idx, &v_err) == false) {
        console.error("Cannot find matched pdo.");
        return;
    }
    console.debug("Following pdo selected, target is %.2f, err is %.2f", voltage_target, v_err);
    log_src_pdo(best_matched_idx, src_pdo[best_matched_idx]);
    auto rdo            = build_rdo_from_pdo(src_pdo[best_matched_idx], best_matched_idx, voltage_target);
    *PtrRequestData     = rdo.d32;
    *PtrPowerObjectType = src_pdo[best_matched_idx].GenericPDO.PowerObject;
}

void USBPD::impl_stack_start() {
    stack_started = true;
}

void USBPD::impl_stack_stop() {
    stack_started = false;
}

void USBPD::impl_snk_ready() {
    power_ready = true;
}

bool USBPD::set_voltage(float v_target, float v_allow_min, float v_allow_max) {
    voltage_target = v_target;
    voltage_min    = v_allow_min;
    voltage_max    = v_allow_max;
    return update_voltage_setting();
}

bool USBPD::get_cable_attached() {
    return cable_attached;
}

bool USBPD::get_power_ready() {
    return power_ready;
}
