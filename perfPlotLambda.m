figure('units','normalized','outerposition',[0 0 1 1]);
hold all;
plot(A, ':b');
plot(B, '-g');
plot(C, '--r');
plot(D, '-.m');
ylabel('Ortalama Hata');
xlabel('Iterasyon');
legend('Momentum = 0.7, Ögrenme katsayisi = 0.1', 'Momentum = 0.7, Ögrenme katsayisi = 0.4', 'Momentum = 0.7, Ögrenme katsayisi = 0.7', 'Momentum = 0.7, Ögrenme katsayisi = 1.0 (en iyi)');


