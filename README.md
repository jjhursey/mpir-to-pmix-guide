# Resources for Tools Transitioning from MPIR to PMIx

The [MPIR Process Acquisition Interface](https://www.mpi-forum.org/docs/) is an [MPI Forum](https://www.mpi-forum.org/) defined interface for debuggers to interact with MPI programs. The [PMIx Standard](https://pmix.github.io/) contains an alternative and more extensible tool interface.

Some PMIx-enabled launchers do not support the MPIR interface, which can be problematic for tools that have not moved from MPIR to PMIx. This project is targeted at those tools as they make their transition. The MPIR to PMIx project has the following goals:
 * Provide links to resources that help tools transition from MPIR to the PMIx tool interface,
 * Maintain a MPIR Shim written in C (to match the language of OpenPMIx) that provides most of the MPIR interface backed by the PMIx tool interface, and
 * Maintain other example software that can aid tools transition from MPIR to the PMIx tool interface.

The MPIR Shim code in this repository is meant to be an example for tools to reference when transitioning to PMIx. As such the software attempts to be structured for readability and reference. It is a fully functional MPIR Shim (see usability notes below) with an rudimentary shared library.

For a production oriented version of the MPIR Shim look to the [MPI Shim](https://github.com/openpmix/mpir-shim) repository which contains a C++ version of the tool that has been used in production environments.

The MPIR Shim does not provide debugging capabilities by itself. It merely provides the symbols and back end functionality to support tools that may choose to use those systems.

## Documentation

 * PMIx Standard v4 and later contain the necessary functionality for PMIx tools: https://github.com/pmix/pmix-standard/
 * The production oriented MPIR Shim: https://github.com/openpmix/mpir-shim

## Building the MPIR Shim

What you will need:
 * [OpenPMIx](https://openpmix.github.io/) v4.x or later installation (though any PMIx v4 standard compliant implementation can be used)

```
./configure --prefix=/path-to-install/mpir-shim --with-pmix=/path-to-openpmix-install
make
make install
```

This will create the `mpirc` binary which can be used to wrap the native launcher.

Additionally, a library is created (`libmpirshim` - both static and shared versions) that can be linked into a launcher library that wants to hide the use the shim from the user.


## Running the MPIR Shim

The MPIR Shim works in a few different modes, namely:
 * Proxy mode indirect launch
 * Non-proxy mode indirect launch
 * Attach mode

Both the _proxy_ and _non-proxy_ modes operate in what the PMIx Standard refers to as an _indirect launch_ model. This means that they rely on a third-party launcher to start the application. This is in contrast to the _direct launch_ model in which the `mpirc` tool would call `PMIx_Spawn` directly to launch the application without the assistance of a third-party launcher. At the moment, only the _indirect launch_ model is used by the `mpirc` shim, but future versions may be extended to support the _direct launch_ model.

### Running in Proxy Mode

**Proxy Mode** : Running the MPIR Shim in a runtime environment where there are no persistent daemons.
For example, `prterun` or `mpirun` used to launch a single job. Those launchers are responsible for launching the daemons, then the application, then cleaning it all up when the job is finished.

Example of MPIR Shim being used without a debugger attached:
```
mpirc mpirun -np 2 ./a.out
```

Legacy tools may want to just prefix the `mpirun` command with `mpirc` in their normal launch model. In the example below we use a fake tool called `mytool` for illustration purposes only.

```
# Launching without the MPIR Shim
mytool -- mpirun -n 8 ./a.out
# Launching with the MPIR Shim
mytool -- mpirc mpirun -n 8 ./a.out
```

### Running in Non-Proxy Mode

**Non-Proxy Mode** : Running the MPIR Shim in a runtime environment with a persistent daemon.
For example, if you started the PRRTE `prte` process to setup a persistent Distributed Virtual Machine (DVM) environment then use `prun` to launch jobs against the DVM environment.

```
prte --daemonize
mpirc -n prun -np 2 ./a.out
pterm
```

### Running in Attach Mode

**Attach Mode** : Running the MPIR Shim as a front end to attach to a running job by referencing the PMIx server by its PID.

Launch your application (possibly without the MPIR shim)
```
mpirun -np 2 ./a.out
```

Later, attach to the running job by using the PID of `mpirun` (using `1234` for illustration below):
```
mpirc -c 1234
# Or directly with a debugger
mytool -- mpirc -c 1234
```

The MPIR Shim will extract the `MPIR_proctable` and idle until the application terminates. You can then use a parallel debugger to connect to the `mpirc` process to read the process table and attach to the remote processes.

Note that Attach Mode assumes a Proxy Mode launch at this time. It may not work with the Non-Proxy mode.

## Support

If you have questions or need help either post a GitHub issue (if it is a problem) or email the [OpenPMIx mailing list](https://groups.google.com/forum/?utm_medium=email&utm_source=footer#!forum/pmix).
