#include "ntddk.h"

PDRIVER_OBJECT DriverObject;
UNICODE_STRING DriverName, DriverSymbol;


VOID Unload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("Driver unloaded \r\n");

	IoDeleteSymbolicLink(&DriverSymbol);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	DbgPrint("Driver has been loaded");

	RtlInitUnicodeString(&DriverName, "L\\Device\\theog");
	RtlInitUnicodeString(&DriverSymbol, "L\\DosDevices\\theog");

	IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DriverObject);
	IoCreateSymbolicLink(&DriverSymbol, &DriverName);
	DriverObject->Flags |= DO_DIRECT_IO;
	DriverObject->Flags &= ~DO_DEVICE_INITIALIZING;
	DriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}


NTSTATUS CreateCall(PDRIVER_OBJECT DriverObject, PIRP irp) {
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDRIVER_OBJECT DriverObject, PIRP irp) {
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

typedef struct KERNEL_READ_REQUEST {
	ULONG ProcessID;
	ULONG Address;
	ULONG Response;
	ULONG Size;


} _KERNEL_READ_REQUEST, *PKERNEL_READ_REQUEST;

typedef struct KERNEL_WRITE_REQUEST {
	ULONG ProcessID;
	ULONG Address;
	ULONG Response;
	ULONG Size;
} _KERNEL_WRITE_REQUEST, *PKERNEL_WRITE_REQUEST;