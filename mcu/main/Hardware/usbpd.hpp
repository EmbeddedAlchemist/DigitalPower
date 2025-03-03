#pragma once

#include "usbpd_dpm_conf.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_pdo_defs.h"
#include <vector>

class USBPD {
  public:
    static constexpr std::size_t max_pdo_count = USBPD_MAX_NB_PDO;

  private:
    std::uint32_t port_num;
    bool cable_attached = false;
    bool power_ready    = false;
    bool stack_started  = false;

    std::vector<USBPD_PDO_TypeDef> src_pdo;

    float voltage_target = 5.f;
    float voltage_min    = 5.f;
    float voltage_max    = 21.f;
    float req_current    = .0f;

  private:
    static USBPD_SNKRDO_TypeDef build_rdo_from_pdo(USBPD_PDO_TypeDef pdo, std::uint32_t pdo_idx, float v_target);
    static void log_src_pdo(std::uint32_t idx, USBPD_PDO_TypeDef pdo);
    static bool find_best_matched_src_pdo(const std::vector<USBPD_PDO_TypeDef> &src_pdo_lst, float voltage_target, float voltage_min, float voltage_max, std::uint32_t *idx, float *v_err);

    bool update_voltage_setting();

  public:
    USBPD(std::uint32_t port_num)
        : port_num(port_num),
          src_pdo(max_pdo_count) {};

    void impl_rcv_src_pdo(uint8_t *ptr, std::uint32_t size);
    void impl_cable_attached();
    void impl_cable_detached();
    void impl_snk_eval_capabilities(uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType);
    void impl_stack_start();
    void impl_stack_stop();
    void impl_snk_ready();

    bool set_voltage(float v_target, float v_allow_min = 5.f, float v_allow_max = 21.f);
    bool get_cable_attached();
    bool get_power_ready();
};