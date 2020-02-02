import serial
import time
#from py_mini_racer import py_mini_racer

ser = serial.Serial('/dev/cu.usbmodem14201',9600)
time.sleep(2)
check_trans= ser.read()
if check_trans == '1':
    print("Dummy Payment Successful.")
    ctx= py_mini_racer.MiniRacer()
    ctx.eval(
        """var fun = () =>  const paymentDataRequest = getGooglePaymentDataRequest();
        paymentDataRequest.transactionInfo = getGoogleTransactionInfo();

        const paymentsClient = getGooglePaymentsClient();
        paymentsClient.loadPaymentData(paymentDataRequest)
            .then(function (paymentData) {
                processPayment(paymentData);
            })
            .catch(function (err) {
        document.getElementById("message").innerHTML=err.statusCode;

                console.error(err);
        });

        function getGooglePaymentDataRequest() {
        const paymentDataRequest = Object.assign({}, baseRequest);
        paymentDataRequest.allowedPaymentMethods = [cardPaymentMethod];
        paymentDataRequest.transactionInfo = getGoogleTransactionInfo();
        paymentDataRequest.merchantInfo = {
            merchantName: 'Example Merchant'
        };
        return paymentDataRequest;
        }

        function getGoogleTransactionInfo() {
        return {
            currencyCode: 'USD',
            totalPriceStatus: 'FINAL',
            totalPrice: '1.00'
        };
        }

        function getGooglePaymentsClient() {
        if (paymentsClient === null) {
            paymentsClient = new google.payments.api.PaymentsClient({ environment: 'TEST' });
        }
        return paymentsClient;
        }
    """)
    ctx.call("fun")
elif check_trans == '0':
    print("Dummy Payment unsuccessful since 2nd authentication step failed.")
