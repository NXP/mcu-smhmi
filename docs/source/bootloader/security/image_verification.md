---
sidebar_position: 2
---

# Image Verification

Image Verification is a mechanism in which we validate that the image running has not been altered either by internal or external factors.

## Application chain of trust
The basis of the security architecture implemented in the SLN-TLHMI-IOT has signed application images. Signing requires the use
of a Certificate Authority (CA). NXP has its own CA for signing applications at the factory, but the CA is not something that is shared
with customers.

The CA is used to create signing entities for applications as shown in the figure below. A certificate from the CA is
stored in the SLN-TLHMI-IOT’s filesystem and is used to verify the signatures of the signing entity certificates. In addition, locally
stored certificates from the signing entities are used to verify the signature of firmware images coming in Over-the-Air (OTA) updates.

```{figure} ../../img/bootloader/App_ChainOfTrust.png
:align: center
:alt: Chain of trust

Chain of trust
```

## Flash Image Configuration Area (FICA) and Image Verification

The FICA table is a section inside the filesystem that is responsible for describing the images that will be booted. It contains
information about the image and signatures of the applications that will be used to ensure that only verified firmware is executed.
This ensures malicious images cannot be executed without it being signed with the certificate authority and certificate that
is programmed into the filesystem. Before any image is jumped to, it is first verified using the signature from its associated
FICA entry.

* The bootloader uses the AppA FICA entry to validate the AppA image
* The bootloader uses the AppB FICA entry to validate the AppB image

```{note}

As mentioned when describing the application banks, `Bank C` is not used for redundancy in the update mechanism, as such, it has no entry into the FICA table. The purpose of the bank is only to showcase all 3 application without the need of reflashing the board.
```

Developers can turn on the image verification and reprogram the bootloader as shown in the Turning on image verification section. To decrease the risks of an attack, have Image Verification on.
