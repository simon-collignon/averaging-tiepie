import csv
import numpy as np
from scipy.optimize import curve_fit
from scipy import signal
import matplotlib
from matplotlib import pyplot as plt

# matplotlib.rcParams.update({
#     "pgf.texsystem": "pdflatex",
#     'font.family': 'serif',
#     'font.size': 14,
#     'text.usetex': True,
#     'pgf.rcfonts': False,
#     'legend.fancybox': False,
#     'legend.shadow': False
# })

class Signal():
    
    def __init__(self, filepath, name=False, color='k', debug=False):
            
        self.filepath = filepath
        self.name = name if name else filepath
        self.color = color
        self.debug = debug
        self.process_header()
        self.process_data()


    def process_header(self):
        ''' returns the header length of the csv file. '''

        is_header = True
        self.header_length = 0
        my_file = open(self.filepath)
        csv_reader = csv.reader(my_file)
        if self.debug: print(f'--- header of {self.filepath} ---')
        while(is_header):
            buff = next(csv_reader)
            
            try:
                float(buff[0])
                is_header = False

            except:
                if self.debug:
                    print([word for word in buff])
                self.header_length += 1
                
        my_file.close() 


    def process_data(self):
        
        data = np.genfromtxt(self.filepath, delimiter=",", skip_header=self.header_length, filling_values=0)
        self.xs = data[:, 0] # time points
        self.ys = data[:, 1] # signal points
        self.duration = self.xs[-1] - self.xs[0] # signal duration
        self.Ts = self.duration / (self.xs.size - 1) # sampling period


my_sig = Signal('record_0.csv', debug=True)


ys = my_sig.ys
xs = my_sig.xs
fs = my_sig.Ts**-1
print(f'sample frequency is {fs}')
# lb=244 # molecular
# ub=-3000

# ll = 20 # chirp
# ul = 230

# b, a = signal.butter(4, 60e6, fs=fs)
# dcys = signal.filtfilt(b, a, ys[lb:ub])

# hpf_ys = ys[lb:ub] -  dcys

# b, a = signal.butter(4, 90e6, fs=fs)
# hpf_ys = signal.filtfilt(b, a, hpf_ys)

plt.figure()
# plt.plot(xs[lb:ub] * 1e6 / fs, hpf_ys * 1e3, 'k', linewidth=1)
# plt.plot(xs[ll:ul] / fs, ys[ll:ul] * 1e3, 'k', linewidth=1)


fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

# Major ticks every 20, minor ticks every 5
major_ticks = np.linspace(-0.4, 0.3998046875, 2**12)

ax.set_yticks(major_ticks)

# And a corresponding grid
ax.grid()

# Or if you want different settings for the grids:
ax.grid(which='major', alpha=0.2)
ax.set_title('Signal')
ax.plot(xs, ys, 'k', linewidth=1)

ax.set_xlabel('Time [$\mu$s]')
ax.set_ylabel('Voltage [mV]')

plt.show()


# yhat = np.abs(np.fft.rfft(hpf_ys))
# xhat = np.fft.rfftfreq(xs[lb:ub].size, (200e6)**-1)

# plt.figure()
# plt.title('Spectrum')

# plt.plot(xhat / 1e6, yhat, 'k', linewidth=1)

# plt.xlabel('Frequency [MHz]')
# plt.ylabel('Intensity [a.u.]')
# plt.tight_layout()

# # plt.show()

# # plt.savefig('cp_noise.pdf')

# plt.figure()
# plt.title('Noise floor spectrum averaged 3,749,900 in $<$ 3 minutes')

# b, a = signal.butter(1, 0.02)

# for i in range(8):
#     my_sig = Signal(f'noise_av10{i}.csv', debug=True)
#     fs = 200 # MHz
#     ys = my_sig.ys
#     xs = my_sig.xs
#     lb=210
#     ub=-1

    
#     dcys = signal.filtfilt(b, a, ys[lb:])

#     hpf_ys = ys[lb:] -  dcys
#     yhat = np.abs(np.fft.rfft(hpf_ys))
#     xhat = np.fft.rfftfreq(xs[lb:].size, (200e6)**-1)

#     plt.plot(xhat / 1e6, yhat, linewidth=1, label=f'N = 10e{i}')
    
# plt.xlabel('Frequency [MHz]')
# plt.ylabel('Intensity [a.u.]')
# plt.legend()
# plt.tight_layout()
# plt.show()
