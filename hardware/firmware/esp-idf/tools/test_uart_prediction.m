function prediction = test_uart_prediction(port, features)
%TEST_UART_PREDICTION Send 21 values to the ESP32 and read its class index.
%   serialportlist("available")
%   prediction = test_uart_prediction("COM5", 1:21)
%   Arbitrary values test communication/inference, not classification accuracy.
%   Close the ESP-IDF serial monitor before opening the same COM port here.

arguments
    port (1,1) string
    features (1,21) double {mustBeFinite} = 1:21
end

device = serialport(port, 115200, "Timeout", 5);
cleanup = onCleanup(@() delete(device));
configureTerminator(device, "LF");
pause(2); % Allow startup if opening the port resets the board.
flush(device);

payload = strjoin(compose("%.17g", features), ",");
writeline(device, payload);
fprintf("Sent 21 values: %s\n", payload);

timer = tic;
while toc(timer) < 10
    if device.NumBytesAvailable == 0
        pause(0.01);
        continue
    end
    response = strtrim(readline(device));
    token = regexp(response, '^PRED,(\d+)$', 'tokens', 'once');
    if ~isempty(token)
        prediction = str2double(token{1});
        assert(prediction >= 0 && prediction <= 10, 'Invalid class index.');
        fprintf("ESP32 prediction index: %d\n", prediction);
        return
    end
    fprintf("ESP32: %s\n", response); % Skip startup/console log lines.
end
error('No PRED response received. Check flashed firmware and COM port.');
end
