Import("env", "projenv" )

libs=env.GetProjectOption("lib_deps")

archsToCheck = ['YauS-stm32f1xx']

for lib in libs:
    for ext in archsToCheck:
        if ext in lib:
            env.Append(CPPDEFINES=[
                "YAUS_USE_ARCH",
            ])

    if "YauS-console" in lib:
        env.Append(CPPDEFINES=[
            "YAUS_USE_MODULE_CONSOLE",
        ])

    if "YauS-drivers" in lib:
        env.Append(CPPDEFINES=[
            "YAUS_USE_MODULE_DRIVERS",
        ])




