figure('units','normalized','outerposition',[0 0 1 1]);
hold all;
plot(A, ':b');
plot(B, '-g');
plot(C, '--r');
plot(D, '-.m');
ylabel('Ortalama Hata');
xlabel('Iterasyon');
legend('Ögrenme katsayisi = 1.0, Momentum = 0.5', 'Ögrenme katsayisi = 1.0, Momentum = 0.6', 'Ögrenme katsayisi = 1.0, Momentum = 0.7 ( en iyi )', 'Ögrenme katsayisi = 1.0, Momentum = 0.8');


