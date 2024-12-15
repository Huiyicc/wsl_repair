#include <iostream>
#include <windows.h>

std::string GetRegistryStringValue(HKEY hKeyRoot, const std::string &subKey, const std::string &valueName) {
  HKEY hKey;
  std::string value;
  DWORD valueLength = 0;

  // 打开注册表项
  if (RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
    // 查询值的大小
    RegQueryValueExA(hKey, valueName.c_str(), nullptr, nullptr, nullptr, &valueLength);
    if (valueLength > 0) {
      value.resize(valueLength);
      RegQueryValueExA(hKey, valueName.c_str(), nullptr, nullptr, (LPBYTE) value.data(), &valueLength);
    }
    RegCloseKey(hKey);
  }
  return value;
}

void SetRegistryStringValue(HKEY hKeyRoot, const std::string &subKey, const std::string &valueName,
                            const std::string &value) {
  HKEY hKey;

  // 打开注册表项
  if (RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
    // 设置字符串值
    RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ, (const BYTE *) value.c_str(), value.size() + 1);
    RegCloseKey(hKey);
  }
}


int main() {
  // 获取注册表值
  auto loca = GetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\Order)",
                                     "ProviderOrder");
  auto locb = GetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\HwOrder)",
                                     "ProviderOrder");

  // 输出注册表值
  std::cout << "start" << std::endl;
  std::cout << R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order -> )" << loca << std::endl;
  std::cout << R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\HwOrder -> )" << locb
            << std::endl;

  // 写入注册表值
  SetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\Order)",
                         "ProviderOrder", "P9NP,RDPNP,LanmanWorkstation,webclient");
  SetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\HwOrder)",
                         "ProviderOrder", "P9NP,RDPNP,LanmanWorkstation,webclient");

  loca = GetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\Order)",
                                "ProviderOrder");
  locb = GetRegistryStringValue(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\NetworkProvider\HwOrder)",
                                "ProviderOrder");

  std::cout << "update set" << std::endl;
  std::cout << R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order -> )" << loca << std::endl;
  std::cout << R"(HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\HwOrder -> )" << locb
            << std::endl;

  std::cout << "end" << std::endl;
  return 0;
}
