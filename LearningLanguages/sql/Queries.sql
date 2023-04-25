-- https://leetcode.com/problems/big-countries
SELECT name, population, area FROM world WHERE population >= 25000000 OR area >= 3000000;

-- https://leetcode.com/problems/recyclable-and-low-fat-products
SELECT product_id from Products where low_fats='Y' and recyclable='Y';

-- https://leetcode.com/problems/find-customer-referee
select name from Customer where referee_id is null or referee_id not in (select id from Customer where id=2);

-- https://leetcode.com/problems/customers-who-never-order/
select name as Customers from Customers where id not in (select customerId from Orders);

-- https://leetcode.com/problems/calculate-special-bonus/
SELECT employee_id, IF(employee_id % 2 = 1 and 'M' != substring(name, 1, 1), salary, 0) AS bonus FROM Employees ORDER BY employee_id;

-- https://leetcode.com/problems/swap-salary
UPDATE Salary SET sex = IF(sex = 'm', 'f', 'm');

-- https://leetcode.com/problems/delete-duplicate-emails/
-- mildly retarded? https://stackoverflow.com/questions/4429319/you-cant-specify-target-table-for-update-in-from-clause
DELETE FROM Person
    WHERE id NOT IN
    (
        SELECT MIN(id) AS MaxId
        FROM (SELECT * FROM Person) as Person2
        GROUP BY email
    );

-- https://leetcode.com/problems/combine-two-tablesB
SELECT p.firstName, p.lastName, a.city, a.state FROM
Person AS p LEFT OUTER JOIN Address as a ON
p.personId = a.personId;

-- 

