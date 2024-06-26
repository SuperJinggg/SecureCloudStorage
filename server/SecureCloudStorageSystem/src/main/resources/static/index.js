var app = angular.module('downloadfileapp', []);
app.controller('downloadfilecontroller', ['$scope' , '$http', '$window', function ($scope , $http, $window) {
    $scope.ng_Func_Downloadfile = function () {
        if (!$window.saveAs) {
            !$window.saveAs && $window.console.log('Your browser dont support ajax download, downloading by default');
            $window.open('/client/SecurityCloudStorageClient.exe');
        } else {
            $http({
                method: 'POST',
                url: '/client/SecurityCloudStorageClient.exe'
            }).then(function successCallback(response) {
                // 请求成功执行代码
                var bin = new $window.Blob([response]);
                deferred.resolve(response);
                $window.saveAs(bin, toFilename);
            }, function errorCallback(response) {
                console.log("failed " + response);
            });
        }
    };
}]);