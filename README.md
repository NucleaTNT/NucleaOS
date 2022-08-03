# Setup

On Windows a WSL Environment should be used. Once in either the WSL environment or within your linux installation:

```
sudo apt update
sudo apt install build-essential mtools qemu-system-x86 qemu-utils
```

<br></br>

# Running

On linux:

-   `make run`

On Windows:

-   Install QEMU
-   Run `kernel/run.bat`

<br></br>

# Troubleshooting

I have had some issues with the latest versions (as of writing this: v4.0.40) of `mtools`, however, downgrading to [v4.0.27](http://ftp.gnu.org/gnu/mtools/mtools_4.0.27_amd64.deb) appears to resolve this issue.

<br></br>

# Credits

Built upon repositories from [AbsurdPoncho](https://github.com/AbsurdPoncho) and guided by their [YouTube OSDev Series](https://youtube.com/playlist?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ).
